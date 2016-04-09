
function setDocunloadCount()
{
	$.ajax({
		url:"https://api.github.com/repos/rusith/genius/releases",
		accept:"application-json",
		success:function(data)
		{
			var dc=0;
			var releases_count=data.length;
			for(i=0;i<releases_count;i++)
			{
				var assets_count=data[i].assets.length;
				for(v=0;v<assets_count;v++)
				{
					dc=dc+data[i].assets[v].download_count;

				}
			}
			$("#downloadcountdisplay").text("downloads : "+dc);
		}
	});
}

setDocunloadCount();