import React, { useState } from "react";
import { FormGroup, FormControl, ControlLabel } from "react-bootstrap";
import "./Login.css";
import LoaderButton from "../components/LoaderButton";

export default function Login(props) {
  const [email, setEmail] = useState("");
  const [password, setPassword] = useState("");
  const [isLoading, setIsLoading] = useState(false);

  function validateForm() {
    return email.length > 0 && password.length > 0;
  }

  function handleSubmit(event) {
    event.preventDefault();
        
    setIsLoading(true);

    fetch('http://127.0.1.1:6502/kj/api/users/signon', {
        headers: {
            'Content-Type':'application/json',
            'Authorization':('Basic '+btoa(email + ":" + password)),
        },
        method: 'GET',     
    }).then(res => {
        console.log(res);        
        props.userHasAuthenticated(true);
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