module.exports = function(minified) {
  var clayConfig = this;
  var _ = minified._;
  var $ = minified.$;
  var HTML = minified.HTML;

	function importPMkey() {
		var PMemail = clayConfig.getItemByMessageKey('pmkEmail').get();
		var PMpin = clayConfig.getItemByMessageKey('pmkPin').get();
		var wuKey = clayConfig.getItemByMessageKey('keyWU').get();
		var owmKey = clayConfig.getItemByMessageKey('keyOWM').get();
		
		try {
			if (PMemail !== "" && PMpin !== "" && PMemail !== null && PMpin !== null && PMemail !== undefined && PMpin !== undefined) {
				if (wuKey === '' || owmKey === '') {							
					 // Set the value of an item based on the userData
					$.request('get', "https://pmkey.xyz/search/?email=" + PMemail + "&pin=" + PMpin)
						.then(function(result) {
						var json = JSON.parse(result);
						if(json.success) {					 
							if(json.keys.weather.wu !== ""){ clayConfig.getItemByMessageKey('keyWU').set(json.keys.weather.wu );}
							if(json.keys.weather.owm !== ""){ clayConfig.getItemByMessageKey('keyOWM').set(json.keys.weather.owm );}
							clayConfig.getItemById('pmkeyText').set("Import Successful.");	
						} else {
							clayConfig.getItemById('pmkeyText').set(json.error );	
						}
				})
				.error(function(status, statusText, responseText) {
				});
				} else clayConfig.getItemById('pmkeyText').set("Both API keys already exist.");	
			} else clayConfig.getItemById('pmkeyText').set("PMkey login details not filled.");	
		} catch (e) {}
	}
	
	function toggleChangelog() {
    if (this.get()) {
			try {
				clayConfig.getItemById('change0').show();
				clayConfig.getItemById('change1').show();
				clayConfig.getItemById('change2').show();
				clayConfig.getItemById('change3').show();
				clayConfig.getItemById('change4').show();
				clayConfig.getItemById('change5').show();
				//clayConfig.getItemById('change6').show();
				//clayConfig.getItemById('change7').show();
				//clayConfig.getItemById('change8').show();
			} catch (e) {}
    } else {
			try {
				clayConfig.getItemById('change0').hide();
				clayConfig.getItemById('change1').hide();
				clayConfig.getItemById('change2').hide();
				clayConfig.getItemById('change3').hide();
				clayConfig.getItemById('change4').hide();
				clayConfig.getItemById('change5').hide();
				//clayConfig.getItemById('change6').hide();
				//clayConfig.getItemById('change7').hide();
				//clayConfig.getItemById('change8').hide();
			} catch (e) {}
    }
  }
	
	function forecastText() {
		try {
			var today = clayConfig.getItemByMessageKey('wConf[6]').get();
			var tomorrow = clayConfig.getItemByMessageKey('wConf[7]').get();
			var prov = clayConfig.getItemByMessageKey('wConf[0]').get();
			var time = new Date();						
			
			if (time.getHours() <= today) { clayConfig.getItemById('sliderText').set("Forecast at current time: Today.");}
			else if ( (time.getHours() >= tomorrow) && (tomorrow >= today) ) {clayConfig.getItemById('sliderText').set("Forecast at current time: Tomorrow.");}
			else {
				if (prov === 2) clayConfig.getItemById('sliderText').set("Forecast at current time: Tonight.");
				else  clayConfig.getItemById('sliderText').set("Forecast at current time: Today.");
			}			
		} catch (e) {}
	}

  clayConfig.on(clayConfig.EVENTS.AFTER_BUILD, function() {
		try {	
			clayConfig.getItemByMessageKey('wConf[2]').hide();
			//clayConfig.getItemById('change0').hide();
			//clayConfig.getItemById('change1').hide();
			//clayConfig.getItemById('change2').hide();
			//clayConfig.getItemById('change3').hide();
			//clayConfig.getItemById('change4').hide();
			//clayConfig.getItemById('change5').hide();
			//clayConfig.getItemById('change6').hide();
			//clayConfig.getItemById('change7').hide();
			//clayConfig.getItemById('change8').hide();
		} catch (e) {}

		try {
			var sliderText1 = clayConfig.getItemByMessageKey('wConf[6]');
			var sliderText2 = clayConfig.getItemByMessageKey('wConf[7]');
			
			forecastText.call(sliderText1);
			sliderText1.on('change', forecastText);
			sliderText2.on('change', forecastText);
		} catch (e) {}
		
		try {
			var changelogToggle = clayConfig.getItemById('changelog');
			toggleChangelog.call(changelogToggle);
			changelogToggle.on('change', toggleChangelog);
		} catch (e) {}
		
		try {
			var importButton = clayConfig.getItemById('import');
			//importPMkey.call(importButton);
			importButton.on('click', importPMkey);
		} catch (e) {}

  });  
};