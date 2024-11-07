export enum Sex {
    MALE = 0,
    FEMALE,
}

export type User = {
    id: Number;
    firstName: String;
    lastName: String;
    height: Number;
    sex: String;
}

export type BIA = {
    id: Number;
    userId: Number;
    timestamp: Number; // TODO find out best solution for this
    weight: Number;
    muscle_mass: Number;
    fat_mass: Number;
    water_mass: Number;
}
