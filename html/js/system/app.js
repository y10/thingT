import { Http } from "./http";
import { Status } from "./status";

class AppModel {
  static instance = new AppModel();

  #settings;

  /**
   * @arg value {{ name?:string, host?:string, ssid?:string }?}
   * @returns {Promise<{ name:string, host:string, ssid:string }>}
   */
  async settings(value) {
    if (value) {
      if (JSON.stringify(this.#settings) == JSON.stringify(value)) {
        return value;
      }
      const json = await Http.json("post", "api/settings", value);
      if (Object.keys(value).length > 0) {
        this.#settings = JSON.parse(JSON.stringify(json));
      }
    } else if (!this.#settings) {
      const settings = await Http.json("get", "api/settings");
      if (Object.keys(settings).length > 0) {
        this.#settings = JSON.parse(JSON.stringify(settings));
      }
    }
    return { ...this.#settings };
  }

  async friendlyName() {
    const { name } = await this.settings();
    return name;
  }

  async hostname() {
    const { host } = await this.settings();
    return host;
  }

  async ssid() {
    const { ssid } = await this.settings();
    return ssid;
  }

  async resatart() {
    Http.json("POST", "esp/restart").catch();
    await wait(5000);
    reload();
  }

  wait(timeout) {
    return new Promise((done) => {
      setTimeout(done, timeout);
    });
  }

  reload() {
    window.location.reload();
  }
}

let app = null;

if (window.app !== undefined) {
  app = window.app;
} else {
  window.app = app = AppModel.instance;
}

/**
 * @type {AppModel}
 */
export const App = app;
