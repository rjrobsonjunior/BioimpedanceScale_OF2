import { User, Sex } from "@/services/types";
import { Storage } from "@/services/storage"

import { View, Text, TextInput, Button } from "react-native";
import { useState } from "react";

const CreateUser = () => {
    const [firstName, onChangeFirstName] = useState<string>("");
    const [lastName, onChangeLastName] = useState<string>("");
    const [height, onChangeHeight] = useState<string>(""); // TODO: this really shouldn't be a string
    // const [sex, setSex] = useState<Sex>();
    const [sex, onChangeSex] = useState<string>("");
    const [birthDate, onChangeBirthDate] = useState<string>("");

    let db: Storage;
    Storage.getInstance(false)
        .then((result) => {
            db = result;
        });

    return (
        <View
            style ={{
                backgroundColor: "#e0e0e0",
                margin: 10,
                padding: 10,
                borderRadius: 8,
            }}
        >
            <Text style={{ fontWeight: "bold", fontSize: 16, alignSelf: "center"}}>Create User</Text>

            <Text>First Name</Text>
            <TextInput value={ firstName } onChangeText={ text => onChangeFirstName(text) } placeholder="John"/>

            <Text>Last Name</Text>
            <TextInput value={ lastName } onChangeText={ text => onChangeLastName(text) } placeholder="Doe"/>

            <Text>Height</Text>
            <TextInput value={ height } onChangeText={ text => onChangeHeight(text) } placeholder="180cm"/>

            {/* TODO: Implement properly*/}
            <Text>Sex</Text>
            <TextInput value={ sex } onChangeText={ text => onChangeSex(text) } placeholder="M or F"/>

            {/* TODO: Implement properly*/}
            <Text>Date of Birth</Text>
            <TextInput value={ birthDate } onChangeText={ text => onChangeBirthDate(text) } placeholder="01/01/1970"/>

            <Button title="Create User" onPress={ async () => await db.createUser(assembleUser()) }/>
        </View>
    );

    function assembleUser(): User {
        const user: User = {
            id: undefined,
            firstName,
            lastName,
            height: Number(height),
            sex: sex === "M" ? Sex.MALE : Sex.FEMALE,
            dateOfBirth: birthDate,
        };
        return user;
    }
}

export default CreateUser;
