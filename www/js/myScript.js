var msg = "";


$(document).ready(function() {
  console.log("ready!");
});


function sendMessage() {
  msg = document.getElementById("sendy-text").value;
  var str = msg.includes("https://www.youtube.com/watch?v=")
  if (msg == "" || str==false) {
    popup_error();
    console.log("no message found");
    document.getElementById("sendy-text").value = "";
    return;
  }

  console.log("sending " + msg + " to the Yun");
  document.getElementById("sendy-text").value = "";
   $.get("/mailbox/" + encodeURIComponent(msg) );
  popup();
}

function popup() {
  $(".PopUp").removeClass('hidden');
  setTimeout(function() {
    $('.PopUp').addClass('hidden');
  }, 4000);

}

function popup_error() {
  $(".PopUp_error").removeClass('hidden');
  setTimeout(function() {
    $('.PopUp_error').addClass('hidden');
  }, 2000);

}
