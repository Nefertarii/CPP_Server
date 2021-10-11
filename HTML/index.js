var star1 = document.getElementById("bg-stars-1");
var star2 = document.getElementById("bg-stars-2");
var star3 = document.getElementById("bg-stars-3");

function randomNum(min, max) {
    return Math.floor(Math.random() * (max - min) + min);
}

window.onload = function bgstars() {
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

var login = new XMLHttpRequest();
var login_name = document.getElementById("username").value;
var login_passwd = document.getElementById("password").value;
var login_button = document.getElementById("login-button");

var avatar = document.getElementById("avatar-success");
var login_fail = document.getElementsByClassName("Follow-left-Login-fail")[0];
var login_success = document.getElementsByClassName("Follow-left-Login-success")[0];
var login_success_username = document.getElementById("Login-username");

login_button.onclick = function() {
	let login_name = document.getElementById("username").value;
	let login_passwd = document.getElementById("password").value;
	var tmp = login_name + "&" + login_passwd;
	login.open("POST", "login", true);
	login.send(tmp);
}
var returnObj;
login.onreadystatechange = function() {
    if (login.readyState == 4 && login.status == 200) {
        returnObj = eval("(" + login.responseText + ")");
		if (returnObj.Login[0].state === "success") {
			login_fail.style.display = "none";
			login_success.style.display = "block";
			avatar.style.backgroundImage = "url(" + returnObj.Login[1].AccountImage + ")";
			login_success_username.innerHTML = returnObj.Login[1].AccountAlias;
        }
        localStorage;
        str_login_data = JSON.stringify(returnObj);
        localStorage.setItem("Logindata",str_login_data)
	}
};


