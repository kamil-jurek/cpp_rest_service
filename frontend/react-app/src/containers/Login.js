import React, { useState } from "react";
import { FormGroup, FormControl, ControlLabel } from "react-bootstrap";
import LoaderButton from "../components/LoaderButton";
import { useFormFields } from "../libs/hooksLib";
import "./Login.css";

export default function Login(props) {
  const [isLoading, setIsLoading] = useState(false);
  const [fields, handleFieldChange] = useFormFields({
    email: "",
    password: ""
  });


  function validateForm() {
    return fields.email.length > 0 && fields.password.length > 0;
  }

  function handleSubmit(event) {
    event.preventDefault();
        
    setIsLoading(true);

    fetch('http://127.0.1.1:6502/kj/api/users/signon', {
        headers: {
            'Content-Type':'application/json',
            'Authorization':('Basic '+btoa(fields.email + ":" + fields.password)),
        },
        method: 'GET',     
    }).then(res => {
        console.log(res);        
        props.userHasAuthenticated(true);
        sessionStorage.setItem("userHasAuthenticated", 'true')
        props.history.push("/");

    }).catch(err => err);
  }

  return (
    <div className="Login">
      <form onSubmit={handleSubmit}>
        <FormGroup controlId="email" bsSize="large">
          <ControlLabel>Email</ControlLabel>
          <FormControl
            autoFocus
            type="email"
            value={fields.email}
            onChange={handleFieldChange}
          />
        </FormGroup>
        <FormGroup controlId="password" bsSize="large">
          <ControlLabel>Password</ControlLabel>
          <FormControl
            value={fields.password}
            onChange={handleFieldChange}
            type="password"
          />
        </FormGroup>
        <LoaderButton
            block
            type="submit"
            bsSize="large"
            isLoading={isLoading}
            disabled={!validateForm()}
            >
            Login
            </LoaderButton>
      </form>
    </div>
  );
}