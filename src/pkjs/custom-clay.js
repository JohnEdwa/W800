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
	
	function forecastText() {
		try {
			var today = clayConfig.getItemByMessageKey('wConf[6]').get();
			var time = new Date();
			if (time.getHours() < today) {clayConfig.getItemById('sliderText').set("Forecast at current time: Today.");}
			else {clayConfig.getItemById('sliderText').set("Forecast at current time: Tomorrow.");}	
		} catch (e) {}
	}
	
	
	function toggleChangelog() {
		if (this.get()) {
			try { clayConfig.getItemById('change1').show(); } catch (e) {}
			try { clayConfig.getItemById('change2').show(); } catch (e) {}
			try { clayConfig.getItemById('change3').show(); } catch (e) {}
			try { clayConfig.getItemById('change4').show(); } catch (e) {}
			try { clayConfig.getItemById('change5').show(); } catch (e) {}
			try { clayConfig.getItemById('change6').show(); } catch (e) {}
			try { clayConfig.getItemById('change7').show(); } catch (e) {}
			try { clayConfig.getItemById('change8').show(); } catch (e) {}
		} else {
			try { clayConfig.getItemById('change1').hide(); } catch (e) {}
			try { clayConfig.getItemById('change2').hide(); } catch (e) {}
			try { clayConfig.getItemById('change3').hide(); } catch (e) {}
			try { clayConfig.getItemById('change4').hide(); } catch (e) {}
			try { clayConfig.getItemById('change5').hide(); } catch (e) {}
			try { clayConfig.getItemById('change6').hide(); } catch (e) {}
			try { clayConfig.getItemById('change7').hide(); } catch (e) {}
			try { clayConfig.getItemById('change8').hide(); } catch (e) {}
    }
  }
	
	function toggleCredits() {
    if (this.get()) {
			try { clayConfig.getItemById('credits0').show(); } catch (e) {}
			try { clayConfig.getItemById('credits1').show(); } catch (e) {}
			try { clayConfig.getItemById('credits2').show(); } catch (e) {}
			try { clayConfig.getItemById('credits3').show(); } catch (e) {}
    } else {
			try { clayConfig.getItemById('credits0').hide(); } catch (e) {}
			try { clayConfig.getItemById('credits1').hide(); } catch (e) {}
			try { clayConfig.getItemById('credits2').hide(); } catch (e) {}
			try { clayConfig.getItemById('credits3').hide(); } catch (e) {}
    }
  }
	
	function toggleIssues() {
    if (this.get()) {
			try { clayConfig.getItemById('issues0').show(); } catch (e) {}
			try { clayConfig.getItemById('issues1').show(); } catch (e) {}
			try { clayConfig.getItemById('issues2').show(); } catch (e) {}
			try { clayConfig.getItemById('issues3').show(); } catch (e) {}
    } else {
			try { clayConfig.getItemById('issues0').hide(); } catch (e) {}
			try { clayConfig.getItemById('issues1').hide(); } catch (e) {}
			try { clayConfig.getItemById('issues2').hide(); } catch (e) {}
			try { clayConfig.getItemById('issues3').hide(); } catch (e) {}
    }
  }
	
	
  clayConfig.on(clayConfig.EVENTS.AFTER_BUILD, function() {
		try {	
			clayConfig.getItemByMessageKey('wConf[2]').hide();
			//clayConfig.getItemById('change0').hide();
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
			var creditsToggle = clayConfig.getItemById('credits');
			toggleCredits.call(creditsToggle);
			creditsToggle.on('change', toggleCredits);
		} catch (e) {}
		
		try {
			var issuesToggle = clayConfig.getItemById('issues');
			toggleIssues.call(issuesToggle);
			issuesToggle.on('change', toggleIssues);
		} catch (e) {}
		
		try {
			var importButton = clayConfig.getItemById('import');
			//importPMkey.call(importButton);
			importButton.on('click', importPMkey);
		} catch (e) {}

  });  
};