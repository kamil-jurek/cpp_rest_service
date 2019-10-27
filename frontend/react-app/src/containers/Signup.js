import React from "react";
import { Button, FormGroup, FormControl, ControlLabel } from "react-bootstrap";
import { useFormFields } from "../libs/hooksLib";
import "./Signup.css";

export default function Login(props) {
  const [fields, handleFieldChange] = useFormFields({
    email: "",
    password: "",
    name: "",
    lastName: ""
  });

  function validateForm() {
    return fields.email.length > 0 && 
           fields.password.length > 0 && 
           fields.name.length > 0 && 
           fields.lastName.length > 0;
  }

  function handleSubmit(event) {
    event.preventDefault();
      
    fetch('http://127.0.1.1:6502/kj/api/users/signup', {
        headers: {
            'Content-Type':'application/json',
        },
        method: 'POST',     
        body: JSON.stringify({
            "email": fields.email, 
            "password": fields.password, 
            "name": fields.name, 
            "lastName": fields.lastName,
        }),
    }).then(res => {
        console.log(res);
        alert("Logged in");

    }).catch(err => err);
    }

  return (
    <div className="Signup">
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
        <FormGroup controlId="name" bsSize="large">
          <ControlLabel>Name</ControlLabel>
          <FormControl
            value={fields.name}
            onChange={handleFieldChange}
            type="text"
          />
        </FormGroup>
        <FormGroup controlId="lastName" bsSize="large">
          <ControlLabel>Last Name</ControlLabel>
          <FormControl
            value={fields.lastName}
            onChange={handleFieldChange}
            type="text"
          />
        </FormGroup>
        <Button block bsSize="large" disabled={!validateForm()} type="submit">
          Signup
        </Button>
      </form>
    </div>
  );
}