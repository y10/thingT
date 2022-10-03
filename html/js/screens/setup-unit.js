import { jQuery } from "../system/jquery";
import { Http } from "../system/http";
import { App } from "../system/app";


const html = `
<div class="container">
    <h1>General</h1>
    <form method="post" enctype="application/x-www-form-urlencoded" action="/settings">
        <input id='name' name='name' length=32 placeholder='Friendly Name'><br />
        <br />
        <input id='host' name='host' length=32 placeholder='Device Name'><br />
    </form>
</div>
`
const style = `
<style>

.container {
  width: 80vw;
  max-width:300px;
}

h1 {
  position: absolute;
  top: 0;
}

@media screen and (min-height: 730px) {
  h1 { top: 6%; }
}

input {
    padding: 8px;
    font-size: 1em;
    width: 100%;
}

</style>
`;

export class SetupUnitName extends HTMLElement {

  settings = {};

  connectedCallback() {
    this.jQuery = jQuery(this).attachShadowTemplate(style + html, async ($) => {
      this.txtName = $('#name');
      this.txtName.value(await App.friendlyName());
      this.txtName.on('change', this.onNameChange.bind(this));

      this.txtHost = $('#host');
      this.txtHost.value(await App.hostname());
      this.txtHost.on('change', this.onHostChange.bind(this));
    });
  }

  disconnectedCallback() {
    this.jQuery().detach();
  }

  onNameChange(e) {
    this.settings['name'] = this.txtName.value();
  }

  onHostChange() {
    this.settings['host'] = this.txtHost.value();
  }

  onSave(e) {
    if (Object.keys(this.settings).length > 0) {
      e.settings = {...e.settings, ...this.settings};
      e.restartRequested = true;
    }
  }
}