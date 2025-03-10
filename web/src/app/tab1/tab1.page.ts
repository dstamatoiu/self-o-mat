import { Component, OnInit } from '@angular/core';
import { XAPIService } from '../api.service';
import {xtech} from '../protos/api';

import CameraSettings = xtech.selfomat.CameraSettings;

@Component({
  selector: 'app-tab1',
  templateUrl: '../settings.page.html',
  styleUrls: ['../settings.page.scss']
})

export class Tab1Page implements OnInit {

  settings: CameraSettings;
  pageTitle = 'CAMERA.title';

  constructor(readonly apiService: XAPIService) {
  }

  updateCameraSettings(cameraSettings: CameraSettings) {
    if (this.apiService.isUpdating) {
      return;
    }

    if (this.settings == null) {
      this.settings = cameraSettings;
      return;
    }

    const array1 =  xtech.selfomat.CameraSettings.encode(this.settings).finish();
    const array2 =  xtech.selfomat.CameraSettings.encode(cameraSettings).finish();

    if (array1.toString() !== array2.toString()) {
      this.settings = cameraSettings;
    }
  }

  refresh() {
    if (this.apiService.isUpdating) return;
    this.apiService.getCameraSettings().subscribe(data => this.updateCameraSettings(data));
  }

  ngOnInit() {
    if (!this.apiService.isDemo) {
      setInterval(() => {
        this.refresh();
      }, 1000);
    }

    this.refresh();
  }
}
