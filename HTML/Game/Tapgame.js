var start_button = document.getElementsByClassName("Game_start")[0];

start_button.onclick = function() {
    var load = document.getElementById("Game_load");
    load.style.width = "100%";
    start_button.style.display = "none";
    console.log("111");
}
