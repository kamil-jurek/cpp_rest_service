import React, { Component } from 'react';
import logo from './logo.svg';
import './App.css';
import MyForm from './MyForm';

class App extends Component {
  render() {
    return (
      <div className="App">
        <Header/>    
        <Content/>       
        <Header/>
        
      </div>
    );
  }
}

class Header extends React.Component {
  render() {
     return (
      <div className="App-header">
        <img src={logo} className="App-logo" alt="logo" />
        <h2>Welcome to React</h2>
      </div>
     );
  }
}

class SideBar extends React.Component {
  render() {
     return (
      <div className="App-sidebar">
          <a href="#">About</a>
          <a href="#">Services</a>
          <a href="#">Clients</a>
          <a href="#">Contact</a>
      </div>
     );
  }
}


class Content extends React.Component {
  constructor() {
    super();
    this.state = {
      users: []
    };
  }
  
  componentDidMount() {  
    fetch("http://127.0.1.1:6502/kj/api/users")
      .then(res => res.json())
      .then(json => {
        console.log(json.users);
        this.setState({ users: json.users })});   
  }
  
  // componentDidUpdate() {
  //   fetch("http://127.0.1.1:6502/kj/api/users")
  //     .then(res => res.json())
  //     .then(json => {
  //       console.log(json.users);
  //       this.setState({ users: json.users })});
  // }

  render() {
    return (
      <div className="App-content">
        <SideBar/>
        <div className="App-content-bar">
          <p className="App-intro" >
              To get started, edit <code>src/App.js</code> and save to reload.
          </p>
          {/* {this.refresh()} */}
          <UserList  users={this.state.users} />

          <MyForm/>
        </div>
      </div>      
    );
  }
}

class UserList extends React.Component {
  constructor(props) {
    super(props);
    this.state = { activeWord: -1 }
  }
  
  onClickFunction = (idx) => {
    this.setState({activeWord: idx})
  }

  render() {
    return (
      <table className="App-list-group">
        <tbody>
          <tr><th>Name</th><th>Last Name</th><th>Email</th><th>Weight</th></tr>
          {this.props.users.map((user, i)=>
            // <UserTableRow key={i} name={user.name} email={user.email} lastName={user.lastName} weight={user.weight} />
            <tr 
             onClick={this.onClickFunction.bind(null, i)}
             className={`segmentsList${this.state.activeWord === i ? ' selected' : ''}`}
             key={i} >
              <td>{user.name}</td>
              <td>{user.lastName}</td>
              <td>{user.email}</td>
              <td>{user.weight}</td>
            </tr>
          
          )}
       
        </tbody>
      </table>
    );
  }
}

const UserTableRow = ({ name, email, lastName, weight }) => {
  return (
        <tr className="tr-selected" >
           <td>{name}</td>
           <td>{lastName}</td>
           <td>{email}</td>
           <td>{weight}</td>
        </tr>
  );
}

export default App;
