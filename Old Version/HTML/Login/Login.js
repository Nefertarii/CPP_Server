var login = new XMLHttpRequest();
var login_name = document.getElementById("username").value;
var login_passwd = document.getElementById("password").value;
var login_button = document.getElementById("login-button");

login_button.disabled = false;

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


login_button.onclick = function () {
    login_button.className = "submit-wait";
    let login_name = document.getElementById("username").value;
    let login_passwd = document.getElementById("password").value;
    var tmp = login_name + "&" + login_passwd;
    login.open("POST", "login", true);
    console.log(tmp);
    login.send(tmp);
}
var returnObj;
login.onreadystatechange = function () {
    if (login.readyState == 4 && login.status == 200) {
        returnObj = eval("(" + login.responseText + ")");
        if (returnObj.Login[0].state === "success") {
            localStorage;
            str_login_data = JSON.stringify(returnObj);
            localStorage.setItem("Logindata",str_login_data)
            window.alert("登录成功 3S后返回主页");
            setTimeout(function (){
                window.location.href = "http://159.75.51.91:8000/";
            }, 2000);
        } else {
            window.alert("登录失败 密码或邮箱错误");
            valuesubmit.className = "submit-continue";
        }
	} else {
        valuesubmit.className = "submit-continue";
    }
};
login.onreadystatechange = function() {
    if (login.readyState == 4 && login.status == 200) {
        returnObj = eval("(" + login.responseText + ")");
		if (returnObj.Login[0].state === "success") {
			avatar.style.backgroundImage = "url(" + returnObj.Login[1].AccountImage + ")";
			login_success_username.innerHTML = returnObj.Login[1].AccountAlias;
        } 
        localStorage;
        str_login_data = JSON.stringify(returnObj);
        localStorage.setItem("Logindata",str_login_data)
	} 
};