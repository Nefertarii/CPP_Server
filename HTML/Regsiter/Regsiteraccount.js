// function name   AaaBbbCcc
// variable name   aaaBbbCcc


//生成随机数
function randomNum(min, max) {
    return Math.floor(Math.random() * (max - min) + min);
}
//生成随机颜色RGB分量
function randomColor(min, max) {
    var _r = randomNum(min, max);
    var _g = randomNum(min, max);
    var _b = randomNum(min, max);
    return "rgb(" + _r + "," + _g + "," + _b + ")";
}

var star1 = document.getElementById("bg-stars-1");
var star2 = document.getElementById("bg-stars-2");
var star3 = document.getElementById("bg-stars-3");
//20% 35% 55% 85%
var board = document.getElementsByClassName("card-list")[0];
board.style.height = "120px";

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

var inputemail = false;
var inputpasswd = false;
var inputusername = false;
var inputverify = false;
var emaildone = "false";
var passwdone = "false";
var usernamedone = "false";
var verifydone = "false";
var promptLabel = document.getElementsByClassName("badinput-prompt")[0];
var pattern = /^([A-Za-z0-9_\-\.])+\@([A-Za-z0-9_\-\.])+\.([A-Za-z]{2,4})$/;
//✕ ✓ →
var emailInput = document.getElementsByClassName("input-email")[0];
var emailarrow = document.getElementById("arrow-email");
var emailcontinue = document.getElementById("continue-email");
emailInput.addEventListener("keydown", EmailDetect);
function EmailDetect() {
    setTimeout(inline, 500);
    function inline() { 
        let emailvaule = emailInput.value;
        if (pattern.test(emailvaule) == true) {
            emailarrow.innerHTML = "✓";
            emailarrow.style.color = "var(--bg-green)";
            promptLabel.style.visibility = "hidden";
            inputemail = true;
            if (emaildone === "false") {
                emailcontinue.className = "continue-success";
            }
        } else if (emailvaule.length == 0) {
            emailarrow.innerHTML = "→";
            emailarrow.style.color = "var(--bg-pink)";
            promptLabel.style.visibility = "hidden";
            inputemail = false;
        } else {
            emailarrow.innerHTML = "✕";
            emailarrow.style.color = "var(--bg-red)";
            promptLabel.style.visibility = "visible";
            promptLabel.innerHTML = "邮箱不合法或已被使用";
            inputemail = false;
        }
        Alldetect();
    }
}
emailcontinue.onclick = function(){
    if (emaildone === "false") {
        emailcontinue.style.visibility = "hidden";
        board.style.height = "220px";
        emaildone = "true";
    }
}

var passwdInput = document.getElementsByClassName("input-password")[0];
var passwdarrow = document.getElementById("arrow-password");
var passwdcontinue = document.getElementById("continue-password");
passwdInput.addEventListener("keydown", PasswdDetect);
function PasswdDetect() {
    setTimeout(inline, 500);
    function inline() { 
        let passwdvaule = passwdInput.value;
        if (passwdvaule.length >= 8) {
            passwdarrow.innerHTML = "✓";
            passwdarrow.style.color = "var(--bg-green)";
            promptLabel.style.visibility = "hidden";
            inputpasswd = true;
            if (passwdone === "false") {
                passwdcontinue.className = "continue-success";
            }
        } else if (passwdvaule.length == 0) {
            passwdarrow.innerHTML = "→";
            passwdarrow.style.color = "var(--bg-pink)";
            promptLabel.style.visibility = "hidden";
            inputpasswd = false;
        } else {
            passwdarrow.innerHTML = "✕";
            passwdarrow.style.color = "var(--bg-red)";
            promptLabel.style.visibility = "visible";
            promptLabel.innerHTML = "密码不合法(需要至少8个字符)";
            inputpasswd = false;
        }
        Alldetect();
    }
}
passwdcontinue.onclick = function(){
    if (passwdone === "false") {
        passwdcontinue.style.visibility = "hidden";
        board.style.height = "320px";
        passwdone = "true";
    }
}

var usernameInput = document.getElementsByClassName("input-username")[0];
var usernamearrow = document.getElementById("arrow-username");
var usernamecontinue = document.getElementById("continue-username");
usernameInput.addEventListener("keydown", UsernameDetect);
function UsernameDetect() {
    setTimeout(inline, 500);
    function inline() { 
        let usernamevaule = usernameInput.value;
        if (usernamevaule.length >= 4) {
            usernamearrow.innerHTML = "✓";
            usernamearrow.style.color = "var(--bg-green)";
            promptLabel.style.visibility = "hidden";
            inputusername = true;
            if (usernamedone === "false") {
                usernamecontinue.className = "continue-success";
            }
        } else if (usernamevaule.length == 0) {
            usernamearrow.innerHTML = "→";
            usernamearrow.style.color = "var(--bg-pink)";
            promptLabel.style.visibility = "hidden";
            inputusername = false;
        } else {
            usernamearrow.innerHTML = "✕";
            usernamearrow.style.color = "var(--bg-red)";
            promptLabel.style.visibility = "visible";
            promptLabel.innerHTML = "用户名不合法(至少需要4个字符)";
            inputusername = false;
        }
        Alldetect();
    }
}
usernamecontinue.onclick = function(){
    if (usernamedone === "false") {
        usernamecontinue.style.visibility = "hidden";
        board.style.height = "500px";
        usernamedone = "true";
    }
}

var verifycode;
document.getElementsByClassName("div-verifypic")[0].onclick = function(){
    verifycode = drawPic();
    console.log(verifycode);
}
function drawPic() {
    //获取canvas
    var verifycodepic = document.getElementById("canvas-verify");
    var verifystr = "0123456789abcdefghijklmnopqrstuvwxyz"; //随机数库
    var picTxt = ""; //随后获取的随机数
    var verifynums = 4; //长度
    var picwidth = verifycodepic.width;
    var picheight = verifycodepic.height;
    var verifytext = verifycodepic.getContext("2d"); //获取 context 对象

    //随机画原，填充颜色
    verifytext.fillStyle = randomColor(180, 240); //填充画布颜色
    verifytext.fillRect(0, 0, picwidth, picheight); //填充矩形--画画
    verifytext.beginPath();
    verifytext.arc(randomNum(0, picwidth), randomNum(0, picheight), 1, 0, 2 * Math.PI);
    verifytext.fill();
    for (var i = 0; i < verifynums; i++) {
        var x = (picwidth - 20) / verifynums * i + 20;
        var y = randomNum(picheight / 2, picheight);
        var randomdeg = randomNum(-45, 45);
        var txt = verifystr[randomNum(0, verifystr.length)];
        picTxt += txt; //获取一个随机数
        verifytext.fillStyle = randomColor(10, 100); //填充随机颜色
        verifytext.font = randomNum(30, 50) + "px SimHei"; //设置随机数大小，字体为SimHei
        verifytext.translate(x, y); //将当前xy坐标作为原始坐标
        verifytext.rotate(randomdeg * Math.PI / 180); //旋转随机角度
        verifytext.fillText(txt, 0, 0); //绘制填色的文本
        verifytext.rotate(-randomdeg * Math.PI / 180);
        verifytext.translate(-x, -y);
    }
    for (var i = 0; i < verifynums; i++) {
        //定义笔触颜色
        verifytext.strokeStyle = randomColor(90, 180);
        verifytext.beginPath();
        //随机划线--4条路径
        verifytext.moveTo(randomNum(0, picwidth), randomNum(0, picheight));
        verifytext.lineTo(randomNum(0, picwidth), randomNum(0, picheight));
        verifytext.stroke();
    }
    return picTxt; //返回随机数字符串
}
verifycode = drawPic();

var verifyInput = document.getElementsByClassName("input-verifycode")[0];
var verifyarrow = document.getElementById("arrow-verifycode");
var valuesubmit = document.getElementById("continue-submit");
verifyInput.addEventListener("keydown", VerifyDetect);
function VerifyDetect() {
    setTimeout(inline, 500);
    function inline() {
        let verifyvaule = verifyInput.value;
        if (verifyvaule === verifycode) {
            verifyarrow.innerHTML = "✓";
            verifyarrow.style.color = "var(--bg-green)";
            promptLabel.style.visibility = "hidden";
            inputverify = true;
        } else if(verifyvaule.length == 0) {
            verifyarrow.innerHTML = "→";
            verifyarrow.style.color = "var(--bg-pink)";
            promptLabel.style.visibility = "hidden";
            inputverify = false;
        } else {
            verifyarrow.innerHTML = "✕";
            verifyarrow.style.color = "var(--bg-red)";
            promptLabel.style.visibility = "visible";
            promptLabel.innerHTML = "验证码不正确";
            inputverify = false;
        }
        Alldetect();
    }
}
function Alldetect() {
    if (inputemail == true && inputpasswd == true && inputusername == true && inputverify == true) {
        console.log("true");
        valuesubmit.className = "continue-success";
    } else {
        console.log("false");
        valuesubmit.className = "continue";
    }
}

var register = new XMLHttpRequest();
document.getElementById("continue-submit").onclick = function () {
    
	let register_email = emailInput.value;
    let register_passwd = passwdInput.value;
    let register_name = usernameInput.value;
	var info = register_name + "&" + register_email + "&" + register_passwd;
	register.open("POST", "register", true);
	register.send(info);
}
register.onreadystatechange = function() {
	if (register.readyState == 4 && register.status == 200) {
		var returnObj = eval("(" + register.responseText + ")");
		if (returnObj.session == "success") {
			left_info_fail.style.display = "none";
			left_info_success.style.display = "block";
			avatar.style.backgroundImage = "url(" + returnObj.image + ")";
			left_info_success_username.innerHTML = returnObj.username;
		}
	}
};