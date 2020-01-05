function getXMLHttpRequest()
{
    var xmlhttp = null;
    if (window.XMLHttpRequest)//自动检测当前浏览器的版本，如果是 IE5.0 以上的高版本的浏览器
    {// code for IE7+, Firefox, Chrome, Opera, Safari
        xmlhttp=new XMLHttpRequest();//创建请求对象
    }
    else////如果浏览器是底版本的
    {// code for IE6, IE5
        xmlhttp=new ActiveXObject("Microsoft.XMLHTTP");//创建请求对象
    }
    return xmlhttp;//返回请求对象
}
function calc_fun(arg) {
    //获取data1 data2输入的至
    var data1 = document.getElementById("data1").value;
    var data2 = document.getElementById("data2").value;
    
    //判断data1 和 data2是否是数值
    if(isNaN(data1) || isNaN(data2))
    {
        document.getElementById("data1").value="";
        document.getElementById("data2").value="";
        alert("请输入数字");

        return;
    }

    //拼筹请求报文GET
    var url="/cgi-bin/calc.cgi";//"/cgi-bin/calc.cgi?10+20"

    var data ="";
    data += data1;
    arg ==1?(data+="+"):(data+="-");
    data+=data2;

    //创建xmlHttpRequest对象
    var xmlhttp = getXMLHttpRequest();
    xmlhttp.onreadystatechange=function () {
        if(xmlhttp.status==200 && xmlhttp.readyState==4)
        {
            var ret = xmlhttp.responseText;
            document.getElementById("ret").value = ret;
        }
    }
    //open
    xmlhttp.open("POST",url, true);
    //send
    xmlhttp.send(data);
  
}