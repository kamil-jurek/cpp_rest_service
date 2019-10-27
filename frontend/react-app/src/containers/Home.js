import React, { useState, useEffect } from "react";
import { PageHeader, ListGroup, ListGroupItem } from "react-bootstrap";
import { LinkContainer } from "react-router-bootstrap";

export default function Home(props) {
  const [users, setUsers] = useState([]);
  const [isLoading, setIsLoading] = useState(true);

  useEffect(() => {
    async function onLoad() {
      if (!props.isAuthenticated) {
        return;
      }

      await loadUsers();
      
      setIsLoading(false);
    }
  
    onLoad();
  }, [props.isAuthenticated]);
  
  function loadUsers() {
    fetch("http://127.0.1.1:6502/kj/api/users")
        .then(res => res.json())
        .then(json => {
          console.log(json.users);
          setUsers(json.users)
        });   
    }

  function renderUsersList(users) {
    return [{}].concat(users).map((user, i) =>
    i !== 0 ? (
      <LinkContainer key={i} to={`/home`}>
        <ListGroupItem>
          <p>
            {"weight: " + user.weight}
            {"email: " + user.email}          
          </p>
        </ListGroupItem>
      </LinkContainer>
    ) : (
      <LinkContainer key="new" to="/signup">
        <ListGroupItem>
          <h4>
            <b>{"\uFF0B"}</b> Add new user
          </h4>
        </ListGroupItem>
      </LinkContainer>
    )
  );
  }

  function renderLander() {
    return (
      <div className="lander">
        <h1>KJ React App</h1>
        <p>A simple KJ React App</p>
      </div>
    );
  }

  function renderUsers() {
    return (
      <div className="users">
        <PageHeader>Users:</PageHeader>
        <ListGroup>
          {!isLoading && renderUsersList(users)}
        </ListGroup>
      </div>
    );
  }

  return (
    <div className="Home">
      {props.isAuthenticated ? renderUsers() : renderLander()}
    </div>
  );
}