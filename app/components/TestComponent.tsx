import { Storage } from  "@/services/storage"
import { User, Sex } from "@/services/types";

import { View, Text } from "react-native";

type UserProp = {
    user: User;
}

const UserTestComponent = (props: UserProp) => {
    let db: Storage;
    Storage.getInstance(false)
        .then((result) => {
            db = result;
        });
    
    return (
        <View
            style={{
                justifyContent: "flex-start",
                backgroundColor: "#b6d6cc",
                padding: 8,
                borderRadius: 8,
                margin: 8,
            }}
        >
            <View style={{
                justifyContent: "center",
                flexDirection: "row",
            }}>
                <Text style={{ fontSize: 16, fontWeight: "bold" }}>User</Text>
            </View>
            <Text>{`Name: ${props.user.firstName} ${props.user.lastName}`}</Text>
            <Text>{`Height: ${props.user.height}`}</Text>
            <Text>{`Sex: ${props.user.sex === Sex.MALE? "Male" : "Female"}`}</Text>
        </View>
    );
}

export default UserTestComponent;
