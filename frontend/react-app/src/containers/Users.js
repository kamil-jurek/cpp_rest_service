import React, { useState, useEffect } from "react";
import "./Users.css";

export default function Users(props) {
  const [users, setUsers] = useState([]);
  const [activeUser, setActiveUser] = useState(-1);

  useEffect(() => {
    async function onLoad() {
      await loadUsers();
    }
  
    onLoad();
  }, []);
  
  function loadUsers() {
    fetch("http://127.0.1.1:6502/kj/api/users")
        .then(res => res.json())
        .then(json => {
          console.log(json.users);
          setUsers(json.users)
        });   
    }

  function onClickFunction(idx) {
    setActiveUser(idx);
  }

  return (
    <table className="UsersTable">
         <thead>
          <tr>
            <th>UserId</th>
            <th>Name</th>
            <th>Last Name</th>
            <th>Email</th>
            <th>Weight</th>
          </tr>
         </thead>
        <tbody>
          {users.map((user, i)=>
            <tr 
             onClick={onClickFunction.bind(null, i)}
             className={`userRow${activeUser === i ? ' selected' : ''}`}
             key={i} >
              {activeUser === i ? 
                <td>active</td> :
                <td>{user.userId}</td>  
              }
              
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