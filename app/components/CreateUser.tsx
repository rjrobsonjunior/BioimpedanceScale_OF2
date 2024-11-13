import { User, Sex } from "../services/types"

import { View, Text, TextInput, Button } from "react-native";
import { useState } from "react";

const CreateUser = () => {
    const [firstName, onChangeFirstName] = useState<string>("");
    const [lastName, onChangeLastName] = useState<string>("");
    const [height, onChangeHeight] = useState<string>(""); // TODO: this really shouldn't be a string
    const [sex, setSex] = useState<Sex>();
    const [birthDate, onChangeBirthDate] = useState<string>("");

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
            <Text>Date of Birth</Text>
            <TextInput value={ birthDate } onChangeText={ text => onChangeBirthDate(text) } placeholder="01/01/1970"/>

            {/*
                <Button title="Create User" onPress={ async () => await db.createUser(murilo) }/>
            */}
            <Button title="Create - NOT WORKING YET" />
        </View>
    );
}

export default CreateUser;
