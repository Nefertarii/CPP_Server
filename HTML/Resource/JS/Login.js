var login = new XMLHttpRequest();
var login_name = document.getElementById("username").value;
var login_passwd = document.getElementById("password").value;
var login_button = document.getElementById("login-button");
var login_out_button = document.getElementById("login-out");

var avatar = document.getElementById("avatar-success");
var login_fail = document.getElementsByClassName("Follow-left-Login-fail")[0];
var login_success = document.getElementsByClassName("Follow-left-Login-success")[0];
var login_success_username = document.getElementById("Login-username");

var Logindata = localStorage.getItem("Logindata");
Logindata = JSON.parse(Logindata); //转为JSON
console.log(Logindata);
if (Logindata == null) {
    ;
} else {
    if (Logindata.state === "success") {
        login_fail.style.display = "none";
        login_success.style.display = "block";
        avatar.style.backgroundImage = "url(" + Logindata.AccountImage + ")";
        login_success_username.innerHTML = Logindata.AccountAlias;
    }  
}

function change_login_button() { 
    login_button.className = "submit-continue";
} 

login_button.onclick = function () {
	let login_name = document.getElementById("username").value;
    let login_passwd = document.getElementById("password").value;
    login_button.className = "submit-wait";
    var tmp = login_name + "&" + login_passwd;
    login.open("POST", "login", true);
    login.send(tmp);
    
    setTimeout(change_login_button, 7000);
}
var returnObj;
login.onreadystatechange = function () {
    if (login.readyState == 4 && login.status == 200) { 
        returnObj = eval("(" + login.responseText + ")");
		if (returnObj.state === "success") {
			login_fail.style.display = "none";
			login_success.style.display = "block";
			avatar.style.backgroundImage = "url(" + returnObj.AccountImage + ")";
            login_success_username.innerHTML = returnObj.AccountAlias;
            localStorage;
            str_login_data = JSON.stringify(returnObj);
            localStorage.setItem("Logindata", str_login_data)
            return;
        } 
        window.alert("登陆失败，邮箱或密码错误");
    }
};

login_out_button.onclick = function () {
    login_fail.style.display = "block";
    login_success.style.display = "none";
    localStorage.clear();
}