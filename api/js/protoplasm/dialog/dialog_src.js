/*
 * Dialog
 * 
 * Display a pop-up dialog overlay on the current page.
 *
 * Written and maintained by Jeremy Jongsma (jeremy@jongsma.org)
 */
if(typeof Effect == 'undefined') {
	if(typeof Protoplasm != 'undefined')
		Protoplasm.useScriptaculous('effects');
	else
		throw("dialog.js requires including script.aculo.us' effects.js library");
}

var Dialog = {
	active: null
}

Dialog.Base = Class.create({
	
	baseInitialize: function(elt, options) {

		this.options = Object.extend({
			}, options || {});

		this.createComponents();

		this.contents = $(elt);
		this.dialog.appendChild(this.contents);

		this.keyListener = null;

	},

	createComponents: function() {

		this.overlay = $('dialog_overlay');

		if (!this.overlay) {

			this.overlay = new Element('div', { 'id': 'dialog_overlay' });

			this.overlay.setStyle({
					'position': 'fixed',
					'top': 0,
					'left': 0,
					'zIndex': 90,
					'width': '100%',
					'height': '100%',
					'backgroundColor': '#000',
					'display': 'none'
				});

			document.body.appendChild(this.overlay);
			if (!this.options.ignoreClicks)
				this.overlay.on('click', this.close.bind(this));

		}

		this.dialog = $('dialog_box');

		if (!this.dialog) {

			this.dialog = new Element('div', { 'id': 'dialog_box' });

			this.dialog.setStyle({
					'position': 'fixed',
					'display': 'none',
					'zIndex': 91
				});

			if (this.options.width || this.options.height) {
				this.dialog.style.overflow = 'auto';
				if (this.options.width)
					this.dialog.style.width = this.options.width + 'px';
				if (this.options.height)
					this.dialog.style.height = this.options.height + 'px';
			}

			if (this.options.dialogClass)
				this.dialog.addClassName(this.options.dialogClass);

			document.body.appendChild(this.dialog);

		}

	},

	baseShow: function() {

		if (Dialog.active)
			Dialog.active.close();

		Dialog.active = this;

		// Select boxes display on top of other things in old browsers
		$$('select').invoke('hide');

		if(typeof Effect == 'undefined') {
			this.overlay.show();
			this.dialog.show();
		} else {
			new Effect.Appear(this.overlay, { duration: 0.1, from: 0.0, to: 0.3 });
			new Effect.Appear(this.dialog, { duration: 0.1 });
		}

		this.resize();

		this.keyListener = document.on('keydown', function(e) {
			if (!this.options.ignoreEsc && e.keyCode == Event.KEY_ESC)
				this.close();
			if (!(Event.findElement(e, '#dialog_box')
					|| e.shiftKey || e.altKey
					|| e.metaKey || e.ctrlKey))
				Event.stop(e);
		}.bind(this));

	},

	show: function() {
		this.baseShow();
	},

	resize: function() {

		var boxDims = Element.getDimensions(this.dialog);
		var contDims = Element.getDimensions(this.contents);
		var viewDims = document.viewport.getDimensions();

		this.dialog.style.left = ((viewDims.width - boxDims.width)/2) + 'px';

		if (boxDims.height > (viewDims.height - 100)) {
			// Scroll dialog, too tall
			var contDiff = boxDims.height - contDims.height;
			this.dialog.style.top = '50px';
			this.dialog.style.height = (viewDims.height - 100) + 'px';
			this.contents.style.height = (viewDims.height - 100 - contDiff) + 'px';
			this.contents.style.overflow = 'auto';
		} else {
			// Show dialog slightly higher than centered on the page
			this.dialog.style.top = ((viewDims.height - boxDims.height)/2)*.7 + 'px';
		}

	},
	
	close: function() {

		if (this.keyListener) {
			this.keyListener.stop();
			this.keyListener = null;
		}

		if (Dialog.active == this)
			Dialog.active = null;

		if(typeof Effect == 'undefined') {
			this.overlay.hide();
			this.dialog.hide();
			$$('select').invoke('show');
			this.dialog.childElements().invoke('remove');
			if (this.options.onClose)
				this.options.onClose()
		} else {
			new Effect.Fade(this.overlay, { duration: 0.1 });
			new Effect.Fade(this.dialog, {
				duration: 0.1,
				afterFinish: function() {
					while(this.dialog.firstChild)
						this.dialog.firstChild.remove();
					$$('select').invoke('show');
					if (this.options.onClose)
						this.options.onClose()
				}.bind(this)});
		}

	},

	success: function(value) {
		this.close();
		if (this.options.onSuccess)
			this.options.onSuccess(value, this.contents);
	},

	failure: function(value) {
		this.close();
		if (this.options.onFailure)
			this.options.onFailure(value, this.contents);
	}

});

Dialog.HTML = Class.create(Dialog.Base, {
	initialize: function(elt, options) {
		this.baseInitialize(elt, options);
	}
});

Dialog.Ajax = Class.create(Dialog.Base, {
	initialize: function(url, options) {
		this.baseInitialize(new Element('div'), options);
		this.url = url;
	},
	show: function() {

		var loading = '<div style="text-align: center; color: #999;">Loading...</div>';
		if (this.options.loadingIcon)
			loading = '<div style="text-align: center;"><img src="' +
				this.options.loadingIcon + '" /></div><br />' + loading;
			this.contents.update(loading);

		new Ajax.Updater(this.contents, this.url, { 'method': 'get' });

		this.baseShow();

	}
});
