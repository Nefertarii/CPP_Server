var login = new XMLHttpRequest();
var login_name = document.getElementById("username").value;
var login_passwd = document.getElementById("password").value;
var login_button = document.getElementById("login_button");

var avatar = document.getElementById("avatar_success");
var left_info_fail = document.getElementById("left_info_fail");
var left_info_success = document.getElementById("left_info_success");
var left_info_success_username = document.getElementById("left_info_success_username");



login_button.onclick = function() {
	let login_name = document.getElementById("username").value;
	let login_passwd = document.getElementById("password").value;
	var tmp = login_name + "&" + login_passwd;
	login.open("POST", "login", true);
	login.send(tmp);
}
login.onreadystatechange = function() {
	if (login.readyState == 4 && login.status == 200) {
		var returnObj = eval("(" + login.responseText + ")");
		if (returnObj.login == "success") {
			left_info_fail.style.display = "none";
			left_info_success.style.display = "block";
			avatar.style.backgroundImage = "url(" + returnObj.AccountImage + ")";
			left_info_success_username.innerHTML = returnObj.AccountAlias;
		}
	}
};

var star1 = document.getElementById("bg-stars-1");
var star2 = document.getElementById("bg-stars-2");
var star3 = document.getElementById("bg-stars-3");

function randomNum(min, max) {
    return Math.floor(Math.random() * (max - min) + min);
}

window.onload = function bgstars() {
    console.log(123);
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