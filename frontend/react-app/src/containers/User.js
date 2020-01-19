import React, { useState, useEffect } from "react";
import { FormGroup, FormControl} from "react-bootstrap";
import LoaderButton from "../components/LoaderButton";

export default function User(props) {
    const [user, setUser] = useState(null);
    const [name, setName] = useState("");
    const [email, setEmail] = useState("");
    const [userId, setUserId] = useState(0);
    const [lastName, setLastName] = useState("");
    const [weight, setWeight] = useState(0.0);
    const [date, setDate] = useState("30-10-2019");

    const [isLoading, setIsLoading] = useState(false);
    const [isDeleting, setIsDeleting] = useState(false);

  useEffect(() => {
    function loadUser() {
        fetch("http://127.0.1.1:6502/kj/api/users/"+props.match.params.id)
            .then(res => res.json())
            .then(user => {
                console.log(user);
                setUser(user);
                setName(user.name);
                setLastName(user.lastName);
                setWeight(user.weight);
                setEmail(user.email);
                setUserId(user.userId);
                setDate(user.date);

        });   
    }
    loadUser();
  }, [props.match.params.id]);

  function validateForm() {
    return user.userId > 0;
  }

  function handleSubmit(event) {
    event.preventDefault();
    setIsLoading(true);
    fetch("http://127.0.1.1:6502/kj/api/users/"+props.match.params.id, {
        headers: {
            'Content-Type':'application/json',
        },
        method: 'PUT', 
        body: JSON.stringify({ 
            "name": name, 
            "lastName": lastName,
            "weight": parseFloat(weight),
            "date": date,
        }),    
    }).then(res => {
        setIsLoading(false);
        console.log(res);             
    }).catch(err => err);

    
  }

  async function handleDelete(event) {
    event.preventDefault();
  
    const confirmed = window.confirm(
      "Are you sure you want to delete this note?"
    );
  
    if (!confirmed) {
      return;
    }
  
    setIsDeleting(true);
  }

  return (
    <div className="User">
        {user && (
          <div className="">
            <table className="UsersTable">
            <thead>
              <tr>
                <th>UserId</th>
                <th>Name</th>
                <th>Last Name</th>
                <th>Email</th>
                <th>Weight</th>
                <th>Date</th>
              </tr>
             </thead>
            <tbody>
              <tr className='userRow' >
                <td>
                   {userId}                
                </td>  
                <td>
                  <form>
                    <FormGroup controlId="name">
                        <FormControl
                            value={name}
                            type="text"
                            onChange={e => setName(e.target.value)}
                        />
                    </FormGroup>
                  </form>
                </td>
                <td>
                  <form>
                    <FormGroup controlId="lastName">
                        <FormControl
                            value={lastName}
                            type="text"
                            onChange={e => setLastName(e.target.value)}
                        />
                    </FormGroup>
                  </form>
                </td>
                <td>
                  {email}
                  </td>
                  <td>
                  <form>
                    <FormGroup controlId="weight">
                        <FormControl
                            value={weight}
                            type="number" min="0.0" max="200.0"
                            onChange={e => setWeight( e.target.value)}
                        />
                    </FormGroup>
                    </form>
                  </td>
                            
                <td>
                    <FormGroup controlId="date">
                        <FormControl
                            value={date}
                            type="date"
                            onChange={e => setDate(e.target.value)}
                        />
                        </FormGroup>
                   
                </td>
                </tr> 
                <tr className='userRow'>
                    <td>{userId}</td>  
                    <td>{name}</td>
                    <td>{lastName}</td>
                    <td>{email}</td>
                    <td>{weight}</td>
                    <td>{date}</td>
                </tr>    
                <tr className='' >
                  <td>
        <form onSubmit={handleSubmit}>    
            
            <LoaderButton
                block
                type="submit"
                bsSize="large"
                bsStyle="primary"
                isLoading={isLoading}
                disabled={!validateForm()}
        
            >
                 Save
             </LoaderButton>
             </form>
            </td>
            <td>
            <form onSubmit={handleSubmit}>    
         <LoaderButton
                block
               bsSize="large"
               bsStyle="danger"
                 onClick={handleDelete}
             isLoading={isDeleting}
                 >
                Delete
            </LoaderButton> 
            </form>
        </td>
        </tr>
        
        </tbody>
          </table>
    </div>
        )}
    </div>
  );
}