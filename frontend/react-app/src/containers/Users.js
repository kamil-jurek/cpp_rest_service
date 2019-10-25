import React from 'react';
import "./Users.css";

export default function Users(props) {
  return (
    <div className="Users">
      <UserList/>
    </div>
  );
}

class UserList extends React.Component {
  constructor(props) {
    super(props);
    this.state = { 
        activeWord: -1,
        users: [] }
  }
  
    componentDidMount() {  
    fetch("http://127.0.1.1:6502/kj/api/users")
        .then(res => res.json())
        .then(json => {
        console.log(json.users);
        this.setState({ users: json.users })});   
    }

  onClickFunction = (idx) => {
    this.setState({activeWord: idx})
  }

  render() {
    return (
      <table className="UsersTable">
        <tbody>
          <tr><th>Name</th><th>Last Name</th><th>Email</th><th>Weight</th></tr>
          {this.state.users.map((user, i)=>
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