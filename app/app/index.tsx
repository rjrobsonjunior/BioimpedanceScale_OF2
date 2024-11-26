import { Storage } from "../services/storage"
import { User } from "../services/types"
import UserTestComponent from "../components/TestComponent"
import CreateUser from "../components/CreateUser"

import { Button, Text, View, ScrollView } from "react-native";
import { useState } from "react";

// const murilo: User = {
//     id: undefined,
//     firstName: 'Murilo',
//     lastName: 'Unten',
//     height: 191,
//     sex: Sex.MALE,
//     dateOfBirth: '01/01/2000',
// }

export default function Index() {
    const [users, setUsers] = useState<User[]>([]);
    // const [firstName, onChangeFirstName] = useState<string>("");
    // const [lastName, onChangeLastName] = useState<string>("");
    const [loadingDB, setLoadingDB] = useState<boolean>(true);
    let db: Storage;
    Storage.getInstance(false)
        .then((result) => {
            setLoadingDB(false)
            db = result;
        });

    const Users = (): any => {
        const usersJSX:any[] = [];
        users.forEach((user: User) => {
            usersJSX.push(UserTestComponent({user}))
        });
        return (
            <View>
            { usersJSX }
            </View>
        );
    }

    if (loadingDB) {
        return (
            <ScrollView
            contentContainerStyle={{
                flex: 1,
                justifyContent: "center",
                alignItems: "center",
            }}>
                <Text>Loading Database</Text>
            </ScrollView>
        );
    }

    return (
        <ScrollView
        contentContainerStyle={{
            flex: 1,
            // justifyContent: "center",
            alignItems: "center",
        }}>
            <Text>Database loaded</Text>
            <Button title="Get Users" onPress={ async () => setUsers(await db.getUsers()) }/>

            <CreateUser />

            
            {/*
                <UserTestComponent user={ murilo }></UserTestComponent>
            */}
            <Users />
        </ScrollView>
    );
}
