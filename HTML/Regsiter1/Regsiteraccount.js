// function name   AaaBbbCcc
// variable name   aaaBbbCcc


var nameInput = document.getElementsByClassName("input_name");
var nameLabel = document.getElementById("input_error_name");
var emailInput = document.getElementsByClassName("input_email");
var emailLabel = document.getElementById("input_error_email");
var passwdInput = document.getElementsByClassName("input_passwd");
var passwdLabel = document.getElementById("input_error_passwd");
var verifyInput = document.getElementById("input_ver");
var submit_button = document.getElementById('input_submit');

var name = "false";
var email = "false";
var passwd = "false";

nameInput[0].addEventListener("keydown", NameDetect);
emailInput[0].addEventListener("keydown", EmailDetect);
passwdInput[0].addEventListener("keydown", PasswdDetect);
verifyInput.addEventListener("keydown", SubmitDetect);

function NameDetect() {
	nameLabel.style.visibility = "hidden";
	nameInput[0].style.backgroundImage = "url('../Image/gif/delay-16px.gif')";
	setTimeout(inline, 500);
	function inline() {
		if (8 <= nameInput[0].value.length) {
			nameLabel.style.visibility = "visible";
			nameLabel.innerHTML = "用户名 " + nameInput[0].value + " 可以使用";
			nameLabel.className = "input_success";
			nameInput[0].style.borderColor = "#34d058";
			nameInput[0].style.backgroundImage = "url('../Image/png/success.png')";
			name = "true";

		} else if (nameInput[0].value.length == 0) {
			nameLabel.style.visibility = "hidden";
			nameInput[0].style.backgroundImage = "url('none')";
			nameInput[0].style.borderColor = "#d8dee2";
			name = "false";
		} else {
			nameLabel.style.visibility = "visible";
			nameLabel.innerHTML = "用户名 " + nameInput[0].value + " 不合法或已被使用";
			nameLabel.className = "input_error";
			nameInput[0].style.borderColor = "#F97583";
			nameInput[0].style.backgroundImage = "url('../Image/png/error.png')";
			name = "false";
		}
	}
}

function EmailDetect() {
	emailLabel.style.visibility = "hidden";
	emailInput[0].style.backgroundImage = "url('../Image/gif/delay-16px.gif')";
	setTimeout(inline, 500);
	var STATE = "none";
	var pattern = /^([A-Za-z0-9_\-\.])+\@([A-Za-z0-9_\-\.])+\.([A-Za-z]{2,4})$/;
	function inline() {
		if (true == pattern.test(emailInput[0].value)) {
			emailLabel.style.visibility = "hidden";
			emailLabel.className = "input_success"
			emailInput[0].style.borderColor = "#34d058";
			emailInput[0].style.backgroundImage = "url('../Image/png/success.png')";
			email = "true";
		} else if (0 == emailLabel.value) {
			emailLabel.style.visibility = "hidden";
			emailInput[0].style.backgroundImage = "url('none')";
			emailInput[0].style.borderColor = "#d8dee2";
			email = "false";
		} else {
			emailLabel.style.visibility = "visible";
			emailLabel.className = "input_error";
			emailLabel.innerHTML = "邮箱不合法或已被使用";
			emailInput[0].style.borderColor = "#F97583";
			emailInput[0].style.backgroundImage = "url('../Image/png/error.png')";
			email = "false";
		}
	}
}

function PasswdDetect() {
	passwdInput[0].style.backgroundImage = "url('../Image/gif/delay-16px.gif')";
	setTimeout(inline, 500);
	function inline() {
		if (8 <= passwdInput[0].value.length) {
			passwdLabel.style.color = "#34d058";
			passwdInput[0].style.borderColor = "#34d058";
			passwdInput[0].style.backgroundImage = "url('../Image/png/success.png')";
			passwd = "true";
		} else if (passwdInput[0].value.length == 0) {
			passwdInput[0].style.backgroundImage = "url('none')";
			passwdInput[0].style.borderColor = "#d8dee2";
			passwd = "false";
		} else {
			passwdLabel.style.color = "#86181D";
			passwdInput[0].style.borderColor = "#F97583";
			passwdInput[0].style.backgroundImage = "url('../Image/png/error.png')";
			passwd = "false";
		}
	}
}

function SubmitDetect() {
	let register_verify = document.getElementById("input_ver").value;
    console.log(register_verify);
    //name === "true" && email === "true" && passwd === "true" && register_verify === str_random
	if (name === "true" && email === "true" && passwd === "true") {
		submit_button.disabled = false;
		submit_button.className = "input_submit_true";
	} else {
		submit_button.disabled = true;
		submit_button.className = "input_submit_false";
	}
}

/*
//var verification = document.getElementById("verification");
var random = Math.floor(Math.random() * 100);
var str_random;
verification.style.backgroundImage = "url(verification/" + random + ".png)";
var requeset = new XMLHttpRequest();
document.onreadystatechange = function() {
	if (document.readyState == "complete") {
		requeset.open("POST", "verification", true);
		requeset.send(random);
	};
};
*/
requeset.onreadystatechange = function() {
	if (requeset.readyState == 4 && requeset.status == 200) {
		var returnObj = eval("(" + requeset.responseText + ")");
		verification.style.backgroundImage = "url(verification/" + returnObj.random + ".png)";
		str_random = returnObj.random;
		console.log(str_random);
	}
}



var register = new XMLHttpRequest();
var register_name = document.getElementById("username").value;
var register_email = document.getElementById("email").value;
var register_passwd = document.getElementById("password").value;
var register_verify = document.getElementById("input_ver").value;
var register_button = document.getElementById("input_submit");
register_button.onclick = function() {
	let register_name = document.getElementById("username").value;
	let register_email = document.getElementById("email").value;
	let register_passwd = document.getElementById("password").value;
	var tmp = register_name + "&" + register_email + "&" + register_passwd;
	register.open("POST", "register", true);
	register.send(tmp);
}
submit_button.onclick = function() {
	let login_name = document.getElementById("username").value;
	let login_passwd = document.getElementById("password").value;
	var register_value = login_name + "&" + login_passwd;
	login.open("POST", "login", true);
	login.send(register_value);
}

    $(function(){
        var show_num = [];
        draw(show_num);

        $("#canvas").on('click',function(){
            draw(show_num);
        })
        $(".btn").on('click',function(){
            var val = $(".input-val").val().toLowerCase();
            var num = show_num.join("");
            if(val==''){
                alert('请输入验证码！');
            }else if(val == num){
                alert('提交成功！');
                $(".input-val").val('');
                // draw(show_num);

            }else{
                alert('验证码错误！请重新输入！');
                $(".input-val").val('');
                // draw(show_num);
            }
        })
    })

    //生成并渲染出验证码图形
    function draw(show_num) {
        var canvas_width=$('#canvas').width();
        var canvas_height=$('#canvas').height();
        var canvas = document.getElementById("canvas");//获取到canvas的对象，演员
        var context = canvas.getContext("2d");//获取到canvas画图的环境，演员表演的舞台
        canvas.width = canvas_width;
        canvas.height = canvas_height;
        var sCode = "a,b,c,d,e,f,g,h,i,j,k,m,n,p,q,r,s,t,u,v,w,x,y,z,A,B,C,E,F,G,H,J,K,L,M,N,P,Q,R,S,T,W,X,Y,Z,1,2,3,4,5,6,7,8,9,0";
        var aCode = sCode.split(",");
        var aLength = aCode.length;//获取到数组的长度
        
        for (var i = 0; i < 4; i++) {  //这里的for循环可以控制验证码位数（如果想显示6位数，4改成6即可）
            var j = Math.floor(Math.random() * aLength);//获取到随机的索引值
            // var deg = Math.random() * 30 * Math.PI / 180;//产生0~30之间的随机弧度
            var deg = Math.random() - 0.5; //产生一个随机弧度
            var txt = aCode[j];//得到随机的一个内容
            show_num[i] = txt.toLowerCase();
            var x = 10 + i * 20;//文字在canvas上的x坐标
            var y = 20 + Math.random() * 8;//文字在canvas上的y坐标
            context.font = "bold 23px 微软雅黑";

            context.translate(x, y);
            context.rotate(deg);

            context.fillStyle = randomColor();
            context.fillText(txt, 0, 0);

            context.rotate(-deg);
            context.translate(-x, -y);
        }
        for (var i = 0; i <= 5; i++) { //验证码上显示线条
            context.strokeStyle = randomColor();
            context.beginPath();
            context.moveTo(Math.random() * canvas_width, Math.random() * canvas_height);
            context.lineTo(Math.random() * canvas_width, Math.random() * canvas_height);
            context.stroke();
        }
        for (var i = 0; i <= 30; i++) { //验证码上显示小点
            context.strokeStyle = randomColor();
            context.beginPath();
            var x = Math.random() * canvas_width;
            var y = Math.random() * canvas_height;
            context.moveTo(x, y);
            context.lineTo(x + 1, y + 1);
            context.stroke();
        }
    }

    //得到随机的颜色值
    function randomColor() {
        var r = Math.floor(Math.random() * 256);
        var g = Math.floor(Math.random() * 256);
        var b = Math.floor(Math.random() * 256);
        return "rgb(" + r + "," + g + "," + b + ")";
    }