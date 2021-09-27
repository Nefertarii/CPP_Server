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
		if (returnObj.session == "success") {
			left_info_fail.style.display = "none";
			left_info_success.style.display = "block";
			avatar.style.backgroundImage = "url(" + returnObj.image + ")";
			left_info_success_username.innerHTML = returnObj.username;
		}
	}
};
