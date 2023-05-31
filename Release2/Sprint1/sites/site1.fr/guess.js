function isCorrect() {
    let random = Math.random() * 99
    let number = document.getElementById('number').value

    if (number === '-69') {
        document.body.classList.add('animateBG')

        document.body.addEventListener('animationend', function () {
            document.body.classList.remove('animateBG')
        }, {once: true})
    } else if (random === number) {
        document.getElementById('answer').style.color = 'green'
        document.getElementById('answer').innerHTML = "Bien joué !!"
    } else {
        document.getElementById('answer').style.color = 'red'
        document.getElementById('answer').innerHTML = "Dommage, la bonne réponse était " + random
    }
}

document.getElementById('submit').addEventListener('click', isCorrect)
