import React from 'react';

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
        <form onSubmit={this.handleSubmit}>
          <label htmlFor="name">Enter name</label>
          <input id="name" name="name" type="text" /><br></br>
  
          <label htmlFor="email">Enter your email</label>
          <input id="email" name="email" type="email" /><br></br>
  
          <label htmlFor="lastName">Enter your lastName</label>
          <input id="lastName" name="lastName" type="text" /><br></br>

          <label htmlFor="password">Enter your password</label>
          <input id="password" name="password" type="password" /><br></br>

          <button>Send data!</button>
          <label htmlFor="result">{this.state.result}</label>
        </form>
      );
    }
  }

export default MyForm;