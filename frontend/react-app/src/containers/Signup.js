import React, { useState } from "react";
import { Button, FormGroup, FormControl, ControlLabel } from "react-bootstrap";
import "./Signup.css";

export default function Login(props) {
  const [email, setEmail] = useState("");
  const [name, setName] = useState("");
  const [lastName, setLastName] = useState("");
  const [password, setPassword] = useState("");

  function validateForm() {
    return email.length > 0 && password.length > 0;
  }

  function handleSubmit(event) {
    event.preventDefault();
      
    fetch('http://127.0.1.1:6502/kj/api/users/signup', {
        headers: {
            'Content-Type':'application/json',
        },
        method: 'POST',     
        body: JSON.stringify({
            "email": email, 
            "password": password, 
            "name": name, 
            "lastName": lastName,
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
            value={email}
            onChange={e => setEmail(e.target.value)}
          />
        </FormGroup>
        <FormGroup controlId="password" bsSize="large">
          <ControlLabel>Password</ControlLabel>
          <FormControl
            value={password}
            onChange={e => setPassword(e.target.value)}
            type="password"
          />
        </FormGroup>
        <FormGroup controlId="name" bsSize="large">
          <ControlLabel>Name</ControlLabel>
          <FormControl
            value={name}
            onChange={e => setName(e.target.value)}
            type="text"
          />
        </FormGroup>
        <FormGroup controlId="lastName" bsSize="large">
          <ControlLabel>Last Name</ControlLabel>
          <FormControl
            value={lastName}
            onChange={e => setLastName(e.target.value)}
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