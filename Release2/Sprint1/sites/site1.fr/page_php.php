<!DOCTYPE html>
<html lang='fr'>
<head>
    <meta charset="utf-8">
    <link rel="stylesheet" href="style_index.css">
    <title>Page php</title>
</head>

<body>

<header>
    <div>
        <h1>Test php</h1>
    </div>

    <nav>
        <ul>
            <h2 class="menu"><a href="index.html">Accueil</a></h2>
            <h2 class="menu"><a href="page1.html">Page1</a></h2>
            <h2 class="menu"><a href="page_php.php">Page2</a></h2>
            <h2 class="menu"><a href="http://www.site2.fr:8080">Site 2</a></h2>
        </ul>
    </nav>
</header>

<section>
    <h2>Petit quizz de culture générale !!</h2>
    <form action="result_php.php" method="GET">
        <h3>Maths:</h3>
        <p>
            <label for="calcul">Combien vaut (3+18/2)%4 ? </label>
            <input type="number" name="calcul" id="calcul" placeholder="Résultat">
        </p>
        <h3>Physique:</h3>
        <p>
            <label for="vitesse">Quelle est la vitesse de la lumière ? (en m/s) </var> </label>
            <input type="number" name="vitesse" id="vitesse" placeholder="Résultat">
        </p>
        <h3> Sport:</h3>
        <p>
            <label for="kendo">En quoi son fait les sabres de kendo (escrime japonaise)? </var> </label>
            <input type="text" name="kendo" id="kendo" placeholder="Ex: acier">
        </p>
        <h3>Français:</h3>
        <p>
            <label for="conjugaison">Quelle est la bonne conjugaison ? </label><br>
            <label><input type="checkbox" name="reponse[]" value="option1"> nous résolûmes</label><br>
            <label><input type="checkbox" name="reponse[]" value="option2"> nous résolions</label><br>
            <label><input type="checkbox" name="reponse[]" value="option3"> nous résolvûmes</label><br>
            <label><input type="checkbox" name="reponse[]" value="option4"> nous résolvâmes</label><br>
        </p>
        <h3> Chimie:</h3>
        <p>
            <label for="atome">Quel est l'élément chimique correspondant au symbole Na dans le tableau périodique ? </var> </label>
            <input type="text" name="atome" id="atome" placeholder="Ex: hydrogène">
        </p>
        <button type="submit">Envoyer</button>
    </form>

    <h2>Pour les plus chevronnés: </h2>
    <form action="result_php.php" method="POST">
    <p>
        <label for="last">Comment sont appelées les bandes dessinés chinoises:</label>
        <input type="text" name="last" id="last">

    </p>
    </form>
</body>
</html>
