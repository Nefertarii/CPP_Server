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
        console.log(returnObj);
        if (returnObj.Login.state === "success") {
            localStorage;
            str_login_data = JSON.stringify(returnObj);
            localStorage.setItem("Logindata", str_login_data)
            window.alert("登录成功 3S后返回主页");
            setTimeout(function () {
                window.location.href = "http://webwasi.com/";
            }, 2000);
            return;
        }
        window.alert("登录失败 密码或邮箱错误");
        valuesubmit.className = "submit-continue";
    }
}

login.onreadystatechange = function () {
    if (login.readyState == 4 && login.status == 200) {
        returnObj = eval("(" + login.responseText + ")");
        if (returnObj.state === "success") {
            localStorage;
            str_login_data = JSON.stringify(returnObj);
            localStorage.setItem("Logindata", str_login_data)
        }
    }
}