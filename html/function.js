function my_button1(arg)
{       
    if(arg == 1)
    {
        //获取id = usr的数据
        var usr = document.getElementById("usr").value;
        var pwd = document.getElementById("pwd").value;
        
            if(usr == "admin" && pwd == "123456")
         {
              window.location.href = "http://www.baidu.com";

         }
         else
         {
              alert("用户名或密码错误请重新输入");
             document.getElementById("usr").value="";
             document.getElementById("pwd").value="";

         }
         }
        else if(arg == 0)//取消
        {
            document.getElementById("usr").value="";
            document.getElementById("pwd").value="";
        }
    
}
function my_button() 
    {
        alert("让你点你就点啊！快滚！");
    }
function hero_fun()
{

    alert(document.getElementById("hero").options[0]);
    // document.getElementById("hero").selectedIndex
}
function timer_fun(arg){
    var My_timer;
    if(arg == "1"){
        My_timer = setInterval("My_timefun()",1000);
    }
    else if(arg == "0")
    {
        window.clearInterval(My_timer);
    }
}
function My_timefun(){
    var num = Math.random()*100;
    document.getElementById("wendu").innerHTML = num;
}
