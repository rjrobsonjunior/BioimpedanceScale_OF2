export enum Sex {
    MALE = 0,
    FEMALE,
}

export type User = {
    id: number;
    firstName: string;
    lastName: string;
    height: number;
    sex: string;
    dateOfBirth: number; // TODO find out best solution for this
}

export type BIA = {
    id: number;
    userId: number;
    timestamp: number; // TODO find out best solution for this
    weight: number;
    muscle_mass: number;
    fat_mass: number;
    water_mass: number;
}
