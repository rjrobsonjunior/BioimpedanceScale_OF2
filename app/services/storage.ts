import * as SQLite from "expo-sqlite";
import * as FileSystem from "expo-file-system";
import { Asset } from "expo-asset";

import { User } from "./types";

const DB_NAME: string = "sqlite.db";

export class Storage {
    static #instance: Storage;
    private db: SQLite.SQLiteDatabase;
    private dbName: string;

    public static async getInstance(resetOldDB: boolean): Promise<Storage> {
        if (!Storage.#instance) {
            Storage.#instance = await Storage.loadDatabase(resetOldDB);
        }

        return Storage.#instance;
    }

    private constructor() {
        this.dbName = DB_NAME;
        this.db = SQLite.openDatabaseSync(this.dbName);
        this.setupTables();
    }

    public async createUser(user: User) {
        console.log("inside createUser. User: ", user);
        const query = await this.db.prepareAsync(`
            INSERT INTO user (first_name, last_name, height, sex, date_of_birth) values (?, ?, ?, ?, ?);
        `);
        await query.executeAsync(user.firstName, user.lastName, user.height, user.sex, user.dateOfBirth);
        return;
    }

    public async getUser(id: number): Promise<User|null> {
        const query = await this.db.prepareAsync(`
            SELECT id_user, first_name, last_name, height, sex, date_of_birth FROM user
            WHERE id_user = ?;
        `);
        const result = await query.executeAsync(id);
        const userData: any = await result.getFirstAsync();

        const user: User = {
            id: userData.id,
            firstName: userData.first_name,
            lastName: userData.last_name,
            height: userData.height,
            sex: userData.sex,
            dateOfBirth: userData.dateOfBirth,
        };
        return user;
    }

    public async getUsers(): Promise<User[]> {
        const result = await this.db.getAllAsync(`
            SELECT id_user, first_name, last_name, height, sex, date_of_birth FROM user;
        `);
        console.log("here lol");
        result.forEach((user: any) => console.log(user.first_name))

        return result.map<User>((u: any) => {
            const user: User = {
                id: u.id,
                firstName: u.first_name,
                lastName: u.last_name,
                height: u.height,
                sex: u.sex,
                dateOfBirth: u.dateOfBirth,
            };
            return user;
        });
    }

    public async getCurrentUserId(): Promise<number | undefined> {
        const result = await this.db.getFirstAsync<{ userId: number }>(`
            SELECT id_user as userId FROM current_user;
        `)
        return result?.userId;
    }

    public setupTables(): number {
        const result = this.db.runSync(`
            CREATE TABLE IF NOT EXISTS user (
                id_user INTEGER PRIMARY KEY NOT NULL,
                first_name TEXT NOT NULL,
                last_name TEXT NOT NULL,
                height DECIMAL(3,2) NOT NULL,
                sex INTEGER NOT NULL,
                date_of_birth DATE NOT NULL
            );

            CREATE TABLE IF NOT EXISTS bia (
                id_bia INTEGER PRIMARY KEY NOT NULL,
                id_user INTEGER NOT NULL,
                timestamp DATETIME NOT NULL,
                weight FLOAT NOT NULL,
                muscle_mass FLOAT NOT NULL,
                fat_mass FLOAT NOT NULL,
                water_mass FLOAT NOT NULL,
                FOREIGN KEY (id_user) REFERENCES user (id_user)
            );

            CREATE TABLE IF NOT EXISTS current_user (
                id_user NOT NULL,
                FOREIGN KEY (id_user) REFERENCES user (id_user)
            );
        `);
        return result.changes;
    }

    private static async loadDatabase(resetOldDB: boolean): Promise<Storage> {
            const dbName: string = DB_NAME;
            const dbAsset = require(`../assets/database/${dbName}`);
            const dbUri = Asset.fromModule(dbAsset).uri;
            const dbFilePath = `${FileSystem.documentDirectory}SQLite/${dbName}`;

            if(resetOldDB && (await Storage.databaseExists(dbName))) {
                console.log("deleting old database");
                await FileSystem.deleteAsync(dbFilePath);
            }
            else if (!(await Storage.databaseExists(dbName))) {
                await FileSystem.makeDirectoryAsync(
                    `${FileSystem.documentDirectory}SQLite`,
                    { intermediates: true }
                );
                
                await FileSystem.downloadAsync(dbUri, dbFilePath);
            }

        return new Storage();
    }

    static async databaseExists(dbName: string): Promise<boolean> {
        const dbFilePath = `${FileSystem.documentDirectory}SQLite/${dbName}`;
        const fileInfo = await FileSystem.getInfoAsync(dbFilePath);
        return fileInfo.exists;
    }
}
