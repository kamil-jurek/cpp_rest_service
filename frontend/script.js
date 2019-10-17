$(document).ready(function() {
    $.ajax({
        url: "http://127.0.1.1:6502/kj/api/test",
        type: 'GET',
        contentType: 'application/json',
        dataType:'json',
    }).then(function(data) {
       $('.status').append(data.status);
       $('.time').append(data.time);
    });

    // $(function(){
    //     $( "#button" ).click(function() {
    //         $.ajax({
    //             type: "POST",
    //             url: "http://127.0.1.1:6502/kj/api/users/signup",
    //             // The key needs to match your method's input parameter (case-sensitive).
    //             data: $('form#myForm').serialize(),
    //             contentType: "application/json",
    //             dataType: "json",
    //             success: function(data){alert(data);},
    //             failure: function(errMsg) {
    //                 alert(errMsg);
    //             }               
    //         });
    // });
    // })
    
    $("#my_form").submit(function(event){
        event.preventDefault(); //prevent default action 
        var post_url = $(this).attr("action"); //get form action url
        var request_method = $(this).attr("method"); //get form GET/POST method
        var form_data = $(this); //Encode form elements for submission
        var email_val = $('input[name=email]').val();
        var JSONObject= {email: email_val, 
            password:"kamil", 
            name:"kamil", 
            lastName:"K"
       
        };
        console.log(email_val);

        $.ajax({
            url : 'http://127.0.1.1:6502/kj/api/users/signup',
            type: 'post',
            contentType: "application/json",
            dataType: "json",
            data : JSON.stringify(JSONObject),
            async: true,
        }).done(function(response){ //
            $("#server-results").html(response);
        });
    });
    
});

