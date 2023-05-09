function isCorrect() {
    let random = Math.random() * 99
    let number = document.getElementById('number').value

    if (random === number) {
        document.getElementById('answer').style.color = 'green'
        document.getElementById('answer').innerHTML = "Bien joué !!"
    } else {
        document.getElementById('answer').style.color = 'red'
        document.getElementById('answer').innerHTML = "Dommage, la bonne réponse était " + random
    }
}