import React, { useState, useEffect } from "react";
import { Link, withRouter } from "react-router-dom";
import { Nav, Navbar, NavItem } from "react-bootstrap";
import { LinkContainer } from "react-router-bootstrap";
import Routes from "./Routes";
import "./App.css";

function App(props) {
  const [isAuthenticated, userHasAuthenticated] = useState(false);
  const [isAuthenticating, setIsAuthenticating] = useState(true);

  function handleLogout() {
    userHasAuthenticated(false);
    sessionStorage.removeItem('userHasAuthenticated');
    props.history.push("/login");
  }

  useEffect(() => {
    onLoad();
  }, []);
  
  async function onLoad() {
    if (sessionStorage.getItem("userHasAuthenticated")) {
      userHasAuthenticated(true);
    }
  
    setIsAuthenticating(false);
  }

  return (
    !isAuthenticating &&
    <div className="App container">
      <Navbar fluid collapseOnSelect>
        <Navbar.Header>
          <Navbar.Brand>
            <Link to="/">KJ React App</Link>
          </Navbar.Brand>
          <Navbar.Toggle />
        </Navbar.Header>
        <Navbar.Collapse>
          <Nav pullRight>
            <LinkContainer to="/users">
              <NavItem>Users</NavItem>
            </LinkContainer>
            {isAuthenticated
              ? <NavItem onClick={handleLogout}>Logout</NavItem>
              : <React.Fragment>
                  <LinkContainer to="/signup">
                    <NavItem>Signup</NavItem>
                  </LinkContainer>
                  <LinkContainer to="/login">
                    <NavItem>Login</NavItem>
                  </LinkContainer>
                </React.Fragment>
            }
          </Nav>
        </Navbar.Collapse>
      </Navbar>
      <Routes appProps={{ isAuthenticated, userHasAuthenticated }} />
    </div>
  );
}

export default withRouter(App);
