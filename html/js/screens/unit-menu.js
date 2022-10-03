import { jQuery } from "../system/jquery";
import { Http } from "../system/http";
import { Status } from "../system/status";
import { Router } from "../system/router";
import { Version } from "../config";
import { App } from "../system/app";

const template = (self) => `
<style>

.container {
  width: 80vw;
  max-width: 300px;
}

button {
  border: 0;
  border-radius: 0.3rem;
  background-color: var(--secondary-background-color);
  line-height: 2.4rem;
  font-size: 1.2rem;
  width: 100%;
  color: var(--secondary-text-color);
  margin: 8px 0;
}

#update{
  background-color: var(--warn-background-color);
  color: var(--warn-text-color);
}

#reset{
  background-color: var(--alert-background-color);
  color: var(--alert-text-color);
}
</style>

<div class="container">
  <button id="setup">setup</button>
  <button id="update">firmware update</button>
  <button id="reset">factory reset</button>
  <button id="restart">restart</button>
  <br>
  <div align="right">v${Version}</div>
</div>
`;
export class UnitMenu extends HTMLElement {
  connectedCallback() {
    this.jQuery = jQuery(this).attachShadowTemplate(template, ($) => {
      $("#reset").on("click", this.reset.bind(this));
      $("#restart").on("click", this.restart.bind(this));
      $("#update").on("click", this.gotoUpdate.bind(this));
      $("#setup").on("click", this.gotoSetup.bind(this));
    });
  }

  disconnectedCallback() {
    this.jQuery().detach();
  }

  gotoSetup() {
    Router.navigate("setup");
  }

  gotoUpdate() {
    Router.navigate("update");
  }

  async restart() {
    const spinner = Status.wait(10000);
    Http.json("POST", "esp/restart").catch();
    await spinner;
    App.reload();
  }

  async reset() {
    if (
      confirm(
        "Are you sure you want to continue? This will wipe out your settings completely!"
      )
    ) {
      const spinner = Status.wait();
      Http.json("POST", "esp/reset").catch();
      await spinner;
      App.reload();
    }
  }
}
