import { Storage, loadDatabase } from "../services/storage"
import { User, Sex } from "../services/types"
import UserTestComponent from "../components/TestComponent"
import CreateUser from "../components/CreateUser"

import { Button, Text, View, TextInput } from "react-native";
import { useState, useEffect } from "react";

const murilo: User = {
    id: undefined,
    firstName: 'Murilo',
    lastName: 'Unten',
    height: 170,
    sex: Sex.MALE,
    dateOfBirth: '29/01/2004',
}

export default function Index() {
    const [users, setUsers] = useState<User[]>([]);
    const [firstName, onChangeFirstName] = useState<string>("");
    const [lastName, onChangeLastName] = useState<string>("");
    const [db, setDB] = useState<Storage>();

    useEffect(() => {
        loadDatabase(false)
            .then((storage: Storage) => {
                setDB(storage)
            })
            .catch((e) => console.error(e));
    }, []);

    if (!db) {
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

            <CreateUser />

            <UserTestComponent user={ murilo }></UserTestComponent>
        </View>
    );
}
