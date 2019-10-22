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
  
  render() {
    return (
      <div className="App-content">
        <p className="App-intro">
            To get started, edit <code>src/App.js</code> and save to reload.
        </p>
        
        <UserList  users={this.state.users} />

        <MyForm/>

      </div>      
    );
  }
}

class UserList extends React.Component {
  render() {
    return (
      <table className="App-list-group">
        <tbody>
          <tr><th>Name</th><th>Last Name</th><th>Email</th></tr>
          {this.props.users.map((user, i)=><UserTableRow key={i} name={user.name} email={user.email} lastName={user.lastName} />)}
        </tbody>
      </table>
    );
  }
}

const UserTableRow = ({ name, email, lastName }) => {
  return (
        <tr>
           <td>{name}</td>
           <td>{lastName}</td>
           <td>{email}</td>
        </tr>
  );
}

// class MyFirstForm extends React.Component {
//   state = {value: 'blue'}

//   submitForm = () => {
//     console.log(this.state.value); // zawartość inputa
//   }

//   handleChange = (event) => {
//     this.setState({ value: event.target.value });
//   }

//   render() {
//     return (
//       <div className="App-form">
//         <input value={this.state.value} onChange={this.handleChange} />
//         <button onClick={this.submitForm}>Submit</button>
//       </div>
//     );
//   }
// }

export default App;
