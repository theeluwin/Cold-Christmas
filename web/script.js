flag = false;

function init()
{
	
}

function show()
{
	if(!flag)
	{
		$("#header").animate({top:"-=40px"}, 500);
		$("#content").fadeIn(500);
		flag = true;
	}
	
	var query = $("#input").val();
	
	$.get("http://api.wolframalpha.com/v2/query?input=pi&appid=LG6WRP-97ELART58A", function(data){
		//$("#content").text(data);
		alert(data);
	});
}

function test()
{
	var xhr = new XMLHttpRequest();
	xhr.open("GET", "http://www.wolframalpha.com/input/?i=1-2", true);
	document.getElementById("content").innerHTML = xhr.responseText;
}