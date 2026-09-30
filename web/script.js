Module.onRuntimeInitialized = () => { start(); }

const squareB = new Image();
const squareW = new Image();
const immutableB = new Image();
const immutableW = new Image();
const empty = new Image();
const erreur = new Image();

// Declare variables and get elements by ID

const canvas = document.getElementById('gameCanvas');
const ctx = canvas.getContext('2d');
ctx.save();
const buttonRestart = document.getElementById('restart');
const buttonSolve = document.getElementById('solve');
const buttonUndo = document.getElementById('undo');
const buttonRedo = document.getElementById('redo');
const buttonRandom = document.getElementById('random');

// Add event listeners
buttonRestart.addEventListener("click", restartGame);
buttonSolve.addEventListener("click", solveGame);
buttonUndo.addEventListener("click", undoMove);
buttonRedo.addEventListener("click", redoMove);
buttonRandom.addEventListener("click", newRandomGame);
canvas.addEventListener("click", canvasLeftClick);
window.addEventListener('resize', resizeCanvas);

// Define functions
function restartGame() {
Module._restart(g);
printGame(g);
}

function solveGame() {
Module._solve(g);
printGame(g);
}

function undoMove() {
Module._undo(g);
printGame(g);
}

function redoMove() {
Module._redo(g);
printGame(g);
}

function newRandomGame() {
g = Module._new_random(6, 6, false, false);
printGame(g);
}

function canvasLeftClick(event) {
const x = event.pageX - canvas.offsetLeft;
const y = event.pageY - canvas.offsetTop;
const squareSize = canvas.width / Module._nb_cols(g);
const row = Math.floor(y / squareSize);
const col = Math.floor(x / squareSize);
const square = (Module._get_square(g, row, col) + 1) % 3;
Module._play_move(g, row, col, square);
printGame(g);
}

function resizeCanvas() {
const minSize = Math.min(window.innerWidth, window.innerHeight);
canvas.width = minSize * 0.7;
canvas.height = canvas.width;
printGame(g);
}
function printGame(g) {
    var text = "";
    var nb_rows = Module._nb_rows(g);
    var nb_cols = Module._nb_cols(g);
    for (var row = 0; row < nb_rows; row++) {
        for (var col = 0; col < nb_cols; col++) {
            var number = Module._get_number(g, row, col);
            var immutable = Module._is_immutable(g, row, col);
            var empty = Module._is_empty(g, row, col);
            // var error = Module._has_error(g, row, col);
            if (empty)
                text += " ";
            else if (immutable && number == 0)
                text += "W";
            else if (immutable && number == 1)
                text += "B";
            else if (number == 0)
                text += "w";
            else if (nummber == 1)
                text += "b";
            else text += "?";
        }
        text += "\n";
    }

    // put this text in <div> element with ID 'result'
    var elm = document.getElementById('result');
    elm.innerHTML = text;
}

function start() {
    console.log("call start routine");
    var g = Module._new_default();
    const LIGHTBULB = 1;
    Module._play_move(g, 0, 0, LIGHTBULB);
    printGame(g);
    Module._delete(g);
}


squareB.src = 'imgweb/squareB.jpg';
squareW.src = 'imgweb/squareW.jpg';
immutableB.src = 'imgweb/immutableB.jpg';
immutableW.src = 'imgweb/immutableW.jpg';
empty.src = 'imgweb/empty.jpg';
erreur.src = 'imgweb/erreur.jpg';

// Votre code pour dessiner la grille de jeu, gérer les clics et les boutons ici

