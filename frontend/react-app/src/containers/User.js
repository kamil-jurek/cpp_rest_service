import React, { useState, useEffect } from "react";
import { LoaderButton, FormGroup, FormControl} from "react-bootstrap";

export default function User(props) {
    const [user, setUser] = useState(null);
    const [name, setName] = useState("");
    const [email, setEmail] = useState("");
    const [userId, setUserId] = useState(0);
    const [lastName, setLastName] = useState("");
    const [weight, setWeight] = useState(0.0);

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
                </tr> 
                <tr className='userRow'>
                    <td>{userId}</td>  
                    <td>{name}</td>
                    <td>{lastName}</td>
                    <td>{email}</td>
                    <td>{weight}</td>
                </tr>                
            </tbody>
          </table>


        <form onSubmit={handleSubmit}>
            
            {/* <LoaderButton
                block
                type="submit"
                bsSize="large"
                bsStyle="primary"
                isLoading={isLoading}
                disabled={!validateForm()}
            >
                Save
            </LoaderButton> */} 
            </form>
            </div>
        //     {/* <LoaderButton
        //         block
        //         type="submit"
        //         bsSize="large"
        //         bsStyle="primary"
        //         isLoading={isLoading}
        //         disabled={!validateForm()}
        //     >
        //         Save
        //     </LoaderButton> */}
        //     {/* <LoaderButton
        //         block
        //         bsSize="large"
        //         bsStyle="danger"
        //         onClick={handleDelete}
        //         isLoading={isDeleting}
        //         >
        //         Delete
        //     </LoaderButton> */}
        // </form>

        )}
    </div>
  );
}