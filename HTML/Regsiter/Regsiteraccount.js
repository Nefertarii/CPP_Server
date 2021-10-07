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

function randomNum(min, max) {
    return Math.floor(Math.random() * (max - min) + min);
}

window.onload = function bgstars() {
    document.getElementsByClassName("card-list")[0].style.height = "90%";
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










var nameInput = document.getElementsByClassName("input-email")[0];
var nameLabel = document.getElementById("input-error-username");
var emailInput = document.getElementsByClassName("input-password")[0];
var emailLabel = document.getElementById("input-error-email");
var passwdInput = document.getElementsByClassName("input-username")[0];
var passwdLabel = document.getElementById("input-error-passwd");
var verifyInput = document.getElementById("input-ver");
var submit_button = document.getElementById('input-submit');

var inputname = "false";
var inputemail = "false";
var inputpasswd = "false";

nameInput.addEventListener("keydown", NameDetect);
emailInput.addEventListener("keydown", EmailDetect);
passwdInput.addEventListener("keydown", PasswdDetect);
verifyInput.addEventListener("keydown", SubmitDetect);

function NameDetect() {
	nameLabel.style.visibility = "hidden";
	nameInput.style.backgroundImage = "url('../Image/gif/delay-16px.gif')";
	setTimeout(inline, 500);
	function inline() {
		if (8 <= nameInput.value.length) {
			nameLabel.style.visibility = "visible";
			nameLabel.innerHTML = "用户名 " + nameInput.value + " 可以使用";
			nameLabel.className = "input_success";
			nameInput.style.borderColor = "#34d058";
			nameInput.style.backgroundImage = "url('../Image/png/success.png')";
			inputname = true;

		} else if (nameInput.value.length == 0) {
			nameLabel.style.visibility = "hidden";
			nameInput.style.backgroundImage = "url('none')";
			nameInput.style.borderColor = "#d8dee2";
			inputname = false;
		} else {
			nameLabel.style.visibility = "visible";
			nameLabel.innerHTML = "用户名 " + nameInput.value + " 不合法或已被使用";
			nameLabel.className = "input_error";
			nameInput.style.borderColor = "#F97583";
			nameInput.style.backgroundImage = "url('../Image/png/error.png')";
            inputname = false;
        }
    }
}
function EmailDetect() {
	emailLabel.style.visibility = "hidden";
	emailInput.style.backgroundImage = "url('../Image/gif/delay-16px.gif')";
	setTimeout(inline, 500);
	var STATE = "none";
	var pattern = /^([A-Za-z0-9_\-\.])+\@([A-Za-z0-9_\-\.])+\.([A-Za-z]{2,4})$/;
	function inline() {
		if (true == pattern.test(emailInput.value)) {
			emailLabel.style.visibility = "hidden";
			emailLabel.className = "input_success"
			emailInput.style.borderColor = "#34d058";
			emailInput.style.backgroundImage = "url('../Image/png/success.png')";
			inputemail = true;
		} else if (0 == emailLabel.value) {
			emailLabel.style.visibility = "hidden";
			emailInput.style.backgroundImage = "url('none')";
			emailInput.style.borderColor = "#d8dee2";
			inputemail = false;
		} else {
			emailLabel.style.visibility = "visible";
			emailLabel.className = "input_error";
			emailLabel.innerHTML = "邮箱不合法或已被使用";
			emailInput.style.borderColor = "#F97583";
			emailInput.style.backgroundImage = "url('../Image/png/error.png')";
			inputemail = false;
		}
	}
}
function PasswdDetect() {
	passwdInput.style.backgroundImage = "url('../Image/gif/delay-16px.gif')";
	setTimeout(inline, 500);
	function inline() {
		if (8 <= passwdInput.value.length) {
			passwdLabel.style.color = "#34d058";
			passwdInput.style.borderColor = "#34d058";
			passwdInput.style.backgroundImage = "url('../Image/png/success.png')";
			inputpasswd = true;
		} else if (passwdInput.value.length == 0) {
			passwdInput.style.backgroundImage = "url('none')";
			passwdInput.style.borderColor = "#d8dee2";
			inputpasswd = false;
		} else {
			passwdLabel.style.color = "#86181D";
			passwdInput.style.borderColor = "#F97583";
			passwdInput.style.backgroundImage = "url('../Image/png/error.png')";
			inputpasswd = false;
		}
	}
}
function SubmitDetect() {
	let register_verify = document.getElementById("input_ver").value;
    console.log(register_verify);
	if (inputname === true && inputemail === true && inputpasswd === true) {
		submit_button.disabled = false;
		submit_button.className = "input_submit_true";
	} else {
		submit_button.disabled = true;
		submit_button.className = "input_submit_false";
	}
}
var register = new XMLHttpRequest();
var register_button = document.getElementById("input_submit");
submit_button.onclick = function() {
    let register_name = document.getElementById("username").value;
	let register_email = document.getElementById("email").value;
    let register_passwd = document.getElementById("password").value;
	var info = register_name + "&" + register_email + "&" + register_passwd;
	register.open("POST", "register", true);
	register.send(info);
}
document.getElementById("verify-code").onclick = function (click) {
    click.preventDefault();
    drawPic();
};
function drawPic() {
    //获取到元素canvas
    var verifycode = document.getElementById("verify-code");
    var verifystr = "0123456789abcdefghijklmnopqrstuvwxyz"; //设置随机数库
    var picTxt = ""; //随机数
    var verifynums = 4;
    var picwidth = verifycode.width;
    var picheight = verifycode.height;
    var verifytext = verifycode.getContext("2d"); //获取 context 对象

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
        verifytext.font = randomNum(50, 80) + "px SimHei"; //设置随机数大小，字体为SimHei
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
drawPic();