import { Button, Text, View, TextInput } from "react-native";
import { Storage, loadDatabase } from "../services/storage"
import { User, Sex } from "../services/types"
import { useState, useEffect } from "react";

const db = new Storage();
const murilo: User = {
    firstName: 'Murilo',
    lastName: 'Unten',
    height: 170,
    sex: Sex.MALE,
    dateOfBirth: '29/01/2004',
}


export default function Index() {
    const [databaseLoaded, setDatabaseLoaded] = useState<boolean>(false);
    const [users, setUsers] = useState<User[]>([]);
    const [firstName, onChangeFirstName] = useState<string>("");
    const [lastName, onChangeLastName] = useState<string>("");

    useEffect(() => {
        loadDatabase()
            .then(() => {
                setDatabaseLoaded(true);
            })
            .catch((e) => console.error(e));
    }, []);


    if (!databaseLoaded) {
        return (
            <View
                style={{
                    flex: 1,
                    justifyContent: "center",
                    alignItems: "center",
                }}
            >
                <Text>Loading Database</Text>
            </View>
        );
    }

    return (
        <View
        style={{
            flex: 1,
            justifyContent: "center",
            alignItems: "center",
        }}
        >
            <Text>Database loaded</Text>
            <Button title="Get Users" onPress={ async () => setUsers(await db.getUsers()) }/>

            <TextInput value={ firstName } onChangeText={ text => onChangeFirstName(text) } placeholder="First name"/>
            <TextInput value={ lastName } onChangeText={ text => onChangeLastName(text) } placeholder="Last name"/>
            <Button title="Create User" onPress={ async () => await db.createUser(murilo) }/>
        </View>
    );
}
