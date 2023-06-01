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
            if ((!isset($_GET['calcul']) || !isset($_GET['vitesse']) || !isset($_GET['kendo'])) && !isset($_POST['last']))
            {
                echo('Il faut répondre à toutes les questions pour soumettre le formulaire.');

                // Arrête l'exécution de PHP
                return;
            }


            else if (!isset($_POST['last']))
            {//Utilisation de GET
                $score = 0;
                
                if($_GET['calcul'] == 0){
                    echo "Maths: 1/1    Bien joué\n";
                    $score+=1;
                }
                else{
                    echo "Le résultat du calcul était 0. Va falloir réviser...\n";
                }

                if($_GET['vitesse'] == 3e8){
                    echo "Physique: 1/1    Bien joué\n";
                    $score+=1;
                }
                else{
                    echo "La lumière se déplace à 3e8 m/s. Va falloir réviser...\n";
                }

                if(strcmp($_GET['kendo'],"bambou") == 0){
                    echo "Sport: 1/1    Bravo\n";
                    $score+=1;
                }
                else{
                    echo "La réponse était le bambou. Il faut se cultiver un peu. Lisez un peu plus de mangas.\n";
                }

                echo "Votre score final est de: $score/3 .\n";
                if($score == 3){
                    echo " Score parfait. Vous avez des bonnes bases !\n";
                }
                else{
                    echo "C'est pas encore ça hein.\n";
                }
            }

            else
            {
            }

        ?>
        </div>
<section>

</body>
</html>





