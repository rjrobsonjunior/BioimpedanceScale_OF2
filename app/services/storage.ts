import * as SQLite from "expo-sqlite";
import * as FileSystem from "expo-file-system";
import { Asset } from "expo-asset";

import { User } from "./types";

const DB_NAME: string = "sqlite.db";

export class Storage {
    private db: SQLite.SQLiteDatabase;

    public constructor() {
        console.log("inside constructor");
        
        this.db = SQLite.openDatabaseSync(DB_NAME);
        this.setup();
    }

    public async createUser(user: User) {
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

    // TODO: look up how to handle this properly
    public async getUsers(): Promise<User[]> {
        const result = await this.db.getAllAsync(`
            SELECT id_user, first_name, last_name, height, sex, date_of_birth FROM user;
        `);
        result.forEach((user: any) => console.log(user.first_name))

        return result.map<User>((userData: any) => {
            const user: User = {
                id: userData.id,
                firstName: userData.first_name,
                lastName: userData.last_name,
                height: userData.height,
                sex: userData.sex,
                dateOfBirth: userData.dateOfBirth,
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

    public setup() {
        console.log("inside setup");
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
        console.log(`rows affected: ${result.changes}`);
        // this.db.execSync(`
        //     CREATE TABLE IF NOT EXISTS user (
        //         id_user INTEGER PRIMARY KEY NOT NULL,
        //         first_name TEXT NOT NULL,
        //         last_name TEXT NOT NULL,
        //         height DECIMAL(3,2) NOT NULL,
        //         sex INTEGER NOT NULL,
        //         date_of_birth DATE NOT NULL
        //     );
        //
        //     CREATE TABLE IF NOT EXISTS bia (
        //         id_bia INTEGER PRIMARY KEY NOT NULL,
        //         id_user INTEGER NOT NULL,
        //         timestamp DATETIME NOT NULL,
        //         weight FLOAT NOT NULL,
        //         muscle_mass FLOAT NOT NULL,
        //         fat_mass FLOAT NOT NULL,
        //         water_mass FLOAT NOT NULL,
        //         FOREIGN KEY (id_user) REFERENCES user (id_user)
        //     );
        //
        //     CREATE TABLE IF NOT EXISTS current_user (
        //         id_user NOT NULL,
        //         FOREIGN KEY (id_user) REFERENCES user (id_user)
        //     );
        // `);
    }
}


export const loadDatabase = async () => {
        const dbName: string = "sqlite.db";
        const dbAsset = require(`../assets/${dbName}`);
        const dbUri = Asset.fromModule(dbAsset).uri;
        const dbFilePath = `${FileSystem.documentDirectory}database/${dbName}`;
        console.log(dbFilePath);

        const fileInfo = await FileSystem.getInfoAsync(dbFilePath);
        if (fileInfo.exists) {
            await FileSystem.deleteAsync(dbFilePath);
            FileSystem.readDirectoryAsync
        }
        // if (!fileInfo.exists) {
        //     await FileSystem.makeDirectoryAsync(
        //         `${FileSystem.documentDirectory}database`,
        //         { intermediates: true }
        //     );
        //     await FileSystem.downloadAsync(dbUri, dbFilePath);
        // }

    return;
}
