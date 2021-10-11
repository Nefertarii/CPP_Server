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
    var maxstars = 80;
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


var searchJson = {};getSearchJson();
function getSearchJson(){
    var searchArr = decodeURI(location.search).substr(1).split("&");
    for(var i in searchArr){
        searchJson[searchArr[i].split("=")[0]] = searchArr[i].split("=")[1]
    }
}
$("#productInfo").html("<label>产品代号：</label><span>"+searchJson.productCode+"</span>\
    <label>产品名称：</label><span>"+searchJson.productName+"</span>");
