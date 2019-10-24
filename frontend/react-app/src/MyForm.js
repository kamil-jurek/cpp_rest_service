import React from 'react';
import './App.css';

class MyForm extends React.Component {
    constructor() {
      super();
      this.handleSubmit = this.handleSubmit.bind(this);
      this.state = {result: '?'};
    }
    
    handleChange = (event) => {
        this.setState({ value: event.target.value });
    }

    handleSubmit(event) {
      event.preventDefault();
    //   const data = new FormData(event.target);
    //   data.set('name', data.get('name'));
    //   data.set('email', data.get('email'));
    //   data.set('lastName', data.get('lastName'));
    //   data.set('password', data.get('password'));
    //   console.log(event.target.name.value);
      
    //   const JSONObject= {
    //     "email": event.target.email.value, 
    //     "password": event.target.password.value, 
    //     "name": event.target.name.value, 
    //     "lastName": event.target.lastName.value     
    //     };

    //     console.log(JSONObject);
      fetch('http://127.0.1.1:6502/kj/api/users/signup', {
        headers: {
            'Content-Type':'application/json',
        },
        method: 'POST',
        //mode: 'cors',      
        body: JSON.stringify({
            "email": event.target.email.value, 
            "password": event.target.password.value, 
            "name": event.target.name.value, 
            "lastName": event.target.lastName.value,
        }),
      }).then(res => {
        console.log(res);
        this.setState({ result: res.statusText });

      }).catch(err => err);
    }
  
    render() {
      return (
        <form onSubmit={this.handleSubmit} className="blackbox-form">         
          <table>
          <tbody>
          <tr>Add user:</tr>
          <tr>
            <td className="wide-screen-label-cell">
                <label htmlFor="name">Name</label>
            </td>
            <td className="mobile-screen-label-cell">
                <label htmlFor="name">Name</label>
                <input id="name" name="name" type="text" />
                <span className="errors"></span>
            </td>
          </tr>

          <tr>
            <td className="wide-screen-label-cell">
                <label htmlFor="email">Email</label>
            </td>
            <td className="mobile-screen-label-cell">
                <label htmlFor="email">Email</label>
                <input id="email" name="email" type="email" />
            </td>
          </tr>

          <tr>
            <td className="wide-screen-label-cell">
                <label htmlFor="email">password</label>
            </td>
            <td className="mobile-screen-label-cell">
                <label htmlFor="password">password</label>
                <input id="password" name="password" type="password" />
            </td>
          </tr>

          <tr>
            <td className="wide-screen-label-cell">
                <label htmlFor="lastName">Email</label>
            </td>
            <td className="mobile-screen-label-cell">
                <label htmlFor="lastName">Last Name</label>
                <input id="lastName" name="lastName" type="text" />
            </td>
          </tr>

          <tr>
          <td className="mobile-screen-label-cell">
            <button>Send data!</button>
            <label htmlFor="result">{this.state.result}</label>
            </td>
         </tr>
          
          </tbody>
          </table>
        </form>
      );
    }
  }

export default MyForm;