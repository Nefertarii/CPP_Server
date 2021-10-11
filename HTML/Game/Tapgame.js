var star1 = document.getElementById("bg-stars-1");
var star2 = document.getElementById("bg-stars-2");
var star3 = document.getElementById("bg-stars-3");
function randomNum(min, max) {
    return Math.floor(Math.random() * (max - min) + min);
}
window.onload = function bgstart() {
    var stars1 = "";
    var stars2 = "";
    var stars3 = "";
    var maxstars = 50;
    for (var i = 0; i < maxstars; i++) {
        stars1 += randomNum(100, 2000) + "px " + randomNum(100, 2000) + "px #fff";
        stars2 += randomNum(100, 2000) + "px " + randomNum(100, 2000) + "px #fff";
        stars3 += randomNum(100, 2000) + "px " + randomNum(100, 2000) + "px #fff";
        if (i+1 != maxstars) {
            stars1 += ",";
            stars2 += ",";
            stars3 += ",";
        }
    }
    star1.style.boxShadow = stars1;
    star2.style.boxShadow = stars2;
    star3.style.boxShadow = stars3;
}

var start_button = document.getElementsByClassName("Game_start")[0];

start_button.onclick = function() {
    var load = document.getElementById("Game_load");
    load.style.width = "100%";
    start_button.style.display = "none";
    setTimeout(inline, 1200);
    function inline() {
        load.style.visibility = "hidden";
    }
}
var avatar = document.getElementById("avatar-success");
var login_fail = document.getElementsByClassName("Follow-left-Login-fail")[0];
var login_success = document.getElementsByClassName("Follow-left-Login-success")[0];
var login_success_username = document.getElementById("Login-username");
var Logindata = localStorage.getItem("Logindata");
Logindata = JSON.parse(Logindata); //转为JSON
if (Logindata == null) {
    ;
} else {
    if (Logindata.Login[0].state === "success") {
        login_fail.style.display = "none";
        login_success.style.display = "block";
        avatar.style.backgroundImage = "url(../" + Logindata.Login[1].AccountImage + ")";
        login_success_username.innerHTML = Logindata.Login[1].AccountAlias;
    }  
}
