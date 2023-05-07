var login = new XMLHttpRequest();
var login_name = document.getElementById("username").value;
var login_passwd = document.getElementById("password").value;
var login_button = document.getElementById("login-button");

login_button.disabled = false;

var avatar = document.getElementById("avatar-success");
var login_fail = document.getElementsByClassName("Follow-left-Login-fail")[0];
var login_success = document.getElementsByClassName("Follow-left-Login-success")[0];
var login_success_username = document.getElementById("Login-username");

login_button.onclick = function () {
    login_button.className = "submit-wait";
    let login_name = document.getElementById("username").value;
    let login_passwd = document.getElementById("password").value;
    var tmp = login_name + "&" + login_passwd;
    login.open("POST", "login", true);
    login.send(tmp);
}

var returnObj;
login.onreadystatechange = function () {
    if (login.readyState == 4 && login.status == 200) {
        returnObj = eval("(" + login.responseText + ")");
        if (returnObj.state === "success") {
            localStorage;
            str_login_data = JSON.stringify(returnObj);
            localStorage.setItem("Logindata", str_login_data)
            window.alert("登陆成功");
            setTimeout(function () {
                window.location.href = "http://106.55.10.242:8000/";
            }, 2000);
            return;
        }
    }
    if (login.readyState == 4 && login.status == 403) {
        window.alert("登陆失败，邮箱或密码错误");
        valuesubmit.className = "submit-continue";
    }
};