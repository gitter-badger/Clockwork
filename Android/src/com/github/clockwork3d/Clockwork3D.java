package com.github.clockwork3d;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;

import org.libsdl.app.SDLActivity;

import android.content.Intent;

public class Clockwork extends SDLActivity {

    @Override
    protected boolean onLoadLibrary(ArrayList<String> libraryNames) {
        // Ensure "Clockwork" (in case of Clockwork shared lib type is used) and "ClockworkPlayer" are being sorted to the top of the list
        Collections.sort(libraryNames, new Comparator<String>() {
            private String sortName(String name) {
                return name.startsWith("Clockwork") ? "00_" + name : name;
            }

            @Override
            public int compare(String lhs, String rhs) {
                return sortName(lhs).compareTo(sortName(rhs));
            }
        });

        // Clockwork shared library must always be loaded if available, so exclude it from return result and all list operations below
        int startIndex = "Clockwork".equals(libraryNames.get(0)) ? 1 : 0;

        // Determine the intention
        Intent intent = getIntent();
        String pickedLibrary = intent.getStringExtra(SampleLauncher.PICKED_LIBRARY);
        if (pickedLibrary == null) {
            // Intention for obtaining library names
            String[] array = libraryNames.subList(startIndex, libraryNames.size()).toArray(new String[libraryNames.size() - startIndex]);
            if (array.length > 1) {
                setResult(RESULT_OK, intent.putExtra(SampleLauncher.LIBRARY_NAMES, array));

                // End Clockwork activity lifecycle
                finish();

                // Return false to indicate no library is being loaded yet
                return false;
            } else {
                // There is only one library available, so cancel the intention for obtaining the library name and by not returning any result
                // However, since we have already started Clockwork activity, let's the activity runs its whole lifecycle by falling through to call the super implementation
                setResult(RESULT_CANCELED);
            }
        } else {
            // Intention for loading a picked library name (and remove all others)
            libraryNames.subList(startIndex, libraryNames.size()).clear();
            libraryNames.add(pickedLibrary);
        }

        return super.onLoadLibrary(libraryNames);
    }

}
