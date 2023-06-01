<!DOCTYPE html>
<html lang='fr'>
<head>
    <meta charset="utf-8">
    <title>Page php</title>
</head>

<body>

<header>
    <div>
        <h1>Test php</h1>
    </div>
</header>
<section>
        <ul>
            <h2 class="menu"><a href="index.html">Accueil</a></h2>
            <h2 class="menu"><a href="page1.html">Page1</a></h2>

        </ul>
        <div>
        <?php
//            if ((!isset($_GET['calcul']) || !isset($_GET['foot']) || !isset($_GET['atome']) || !isset($_GET['vitesse']) || !isset($_GET['reponse']) || !isset($_GET['answer']) || !isset($_GET['kendo'])) && !isset($_POST['last']))
//            {
//                echo "Il faut répondre à toutes les questions pour soumettre le formulaire.";
//
//                // Arrête l'exécution de PHP
//                return;
//            }
//

//            else if (!isset($_POST['last']))
            if (1==2)
            {//Utilisation de GET
                $score = 0;
                
                if($_GET['calcul'] == 0){
                    echo "Maths: 1/1    Bien joué<br>";
                    $score+=1;
                }
                else{
                    echo "Le résultat du calcul était 0. Va falloir réviser...<br>";
                }

                if($_GET['vitesse'] == 3e8){
                    echo "Physique: 1/1    Bien joué<br>";
                    $score+=1;
                }
                else{
                    echo "La lumière se déplace à 3e8 m/s. Va falloir réviser...<br>";
                }

                if((strcmp($_GET['kendo'],"bambou") == 0) || (strcmp($_GET['kendo'],"Bambou") == 0)){
                    echo "Sport: 1/1    Bravo<br>";
                    $score+=1;
                }
                else{
                    echo "La réponse était le bambou. Il faut se cultiver un peu. Lisez un peu plus de mangas.<br>";
                }

                if (in_array('option1', $_GET['reponse'])) {
                    echo "Français: 1/1    Bravo<br>";
                    $score+=1;
                }
                else{
                    echo "La réponse était nous résolûmes idiot.<br>";
                }

                if (in_array('option7', $_GET['answer'])) {
                    echo "Géographie: 1/1    Bravo<br>";
                    $score+=1;
                }
                else{
                    echo "La capitale du Sri Lanka est Sri Jayawardenapura Kotte, c'est connu...<br>";
                }

                if((strcmp($_GET['atome'],"sodium") == 0) || (strcmp($_GET['atome'],"Sodium") == 0)){
                    echo "Chimie: 1/1    Bravo<br>";
                    $score+=1;
                }
                else{
                    echo "L'élément Na correspond au sodium. Fais un effort bon sang.<br>";
                }

                if($_GET['foot'] == 2010){
                    echo "Histoire: 1/1    Bien joué<br>";
                    $score+=1;
                }
                else{
                    echo "L'Espagne a gagné en 2010, quel banger d'ailleurs<br>";
                }

                echo "Votre score final est de: $score/7.";
                if($score == 7){
                    echo " Score parfait. Vous avez des bonnes bases !<br>";
                }
                else{
                    echo "Tout le monde n'a pas la science infuse.<br>";
                }
            }

            if (isset($_POST['last']))
            {
                if(strcmp($_POST['last'],"Manhua") != 0){
                    echo "Dommage, vous ferez peut-être mieux la prochaine fois.";
                }
                else{
                    echo "Bravo, j'admire vos connaissances";
                }
            }

            else {
                echo "Il faut répondre à toutes les questions pour soumettre le formulaire.";
                return;
            }

        ?>
        </div>
<section>

</body>
</html>





