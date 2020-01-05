function getXMLHttpRequest() {
	var xmlhttp = null;
	if (window.XMLHttpRequest) //自动检测当前浏览器的版本，如果是 IE5.0 以上的高版本的浏览器
	{ // code for IE7+, Firefox, Chrome, Opera, Safari
		xmlhttp = new XMLHttpRequest(); //创建请求对象
	} else ////如果浏览器是底版本的
	{ // code for IE6, IE5
		xmlhttp = new ActiveXObject("Microsoft.XMLHTTP"); //创建请求对象
	}
	return xmlhttp; //返回请求对象
}

function get_file_fun() {
	//1.创建对象
	var xmlhttp = null;
	xmlhttp = getXMLHttpRequest();
	
	//2.设置回调函数
	xmlhttp.onreadystatechange = function() {
		//判断status == 200 && readState == 4
		//表示服务器处理完成
		//alert(xmlhttp.status);
		if (xmlhttp.status == 200 && xmlhttp.readyState == 4) {
			//服务器的应答
			var ret = xmlhttp.responseText;
			//alert(ret);
			document.getElementById("label").innerHTML = ret;
		}
	}
	
	//3.open
	var url = "a.txt";
//	url = "/cig-bin/led.cgi?on"
	xmlhttp.open("GET",url,true);//true 异步 false同步
	
	
	//4.send
	xmlhttp.send();
}
function calc_fun(arg)
	
{
	var data1 = document.getElementById("data1").value;
	var data2 = document.getElementById("data2").value;
	if(isNaN(data1) || isNaN(data2))
		
	{
		document.getElementById("data1").value = "";
		document.getElementById("data2").value = "";
		
		alert("请输入数字");
		return;
	}
	//拼凑请求报文
	var url = "/cgi-bin/calc.cgi?";
	url +=data1;
	arg == 1 ? ()url += "+") : (url+="-");
	url+=data2;
	alert("url = "+url);
	
	//创建xmlHTTPRequest对象
	var xmlhttp  =getXMLHttpRequest();
	//设置回调函数
		
	xmlhttp.onreadystatechange = function(){
		if(xmlhttp.status == 200 && xmlhttp.readyState == 4)
		{
			var ret = xmlhttp.responseText;
			alert("ret = "+ret);
		}
	}
	//open
	xmlhttp.open("GET",url,true);
	//send
	xmlhttp.send();
}