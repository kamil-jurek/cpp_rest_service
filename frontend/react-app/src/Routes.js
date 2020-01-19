import React from "react";
import { Route, Switch } from "react-router-dom";
import Home from "./containers/Home";
import NotFound from "./containers/NotFound";
import Login from "./containers/Login";
import Signup from "./containers/Signup";
import Users from "./containers/Users";
import User from "./containers/User";
import Galery from "./containers/Galery";
import AppliedRoute from "./components/AppliedRoute";

export default function Routes({ appProps }) {
  return (
    <Switch>
      <AppliedRoute path="/" exact component={Home} appProps={appProps} />
      <AppliedRoute path="/login" exact component={Login} appProps={appProps} />
      <AppliedRoute path="/signup" exact component={Signup} appProps={appProps} />
      <AppliedRoute path="/users" exact component={Users} appProps={appProps} />
      <AppliedRoute path="/users/:id" exact component={User} appProps={appProps} />
      <AppliedRoute path="/galery" exact component={Galery} appProps={appProps} />
      <Route component={NotFound} />
    </Switch>
  );
}