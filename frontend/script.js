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

    $("#my_form").submit(function(event){
        event.preventDefault(); //prevent default action 
        var post_url = $(this).attr("action"); //get form action url
        var request_method = $(this).attr("method"); //get form GET/POST method
        var form_data = $(this); //Encode form elements for submission
        var JSONObject= {
            email: $('input[name=email]').val(), 
            password: $('input[name=password]').val(), 
            name: $('input[name=name]').val(), 
            lastName: $('input[name=lastName]').val()     
        };

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

