import * as SQLite from "expo-sqlite";
import { User } from './types';

const DATABASE_NAME = "app.db";

class Storage {
    private db: SQLite.SQLiteDatabase;

    public constructor() {
        this.db = SQLite.openDatabaseSync(DATABASE_NAME);
        this.setup();
    }

    // TODO: look up how to handle this properly
    public async getUsers(): Promise<User[] | undefined> {
        const result = await this.db.getAllAsync<User>(`
            SELECT id_user, first_name, last_name, height, sex, date_of_birth FROM user;
        `);
        return result;
    }

    public async getCurrentUserId(): Promise<number | undefined> {
        const result = await this.db.getFirstAsync<{ userId: number }>(`
            SELECT id_user as userId FROM current_user;
        `)
        return result?.userId;
    }

    private setup() {
        this.db.execSync(`
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
    }
}
