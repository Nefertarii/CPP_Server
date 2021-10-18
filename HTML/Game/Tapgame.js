function randomNum(min, max) {
    return Math.floor(Math.random() * (max - min) + min);
}
var start_button = document.getElementsByClassName("Game_start")[0];
start_button.onclick = function() {
    var load = document.getElementById("Game_load");
    load.style.width = "100%";
    start_button.style.display = "none";
    setTimeout(inline, 1200);
    function inline() {
        load.style.visibility = "hidden";
    }
}